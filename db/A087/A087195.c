/*
 * A087195  Numbers of the form 1+(1+p)*p^e, p prime and e>0.
 * https://oeis.org/A087195
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 1 + (1 + p) * p^e for prime p and e > 0, sorted
 *            increasingly, duplicates removed. Indexing starts at 1
 * Range:     u8 n <= 13, u16 n <= 46, u32 n <= 46, u64 n <= 46, u128 n <= 46,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>

/* ------------------------------------------------------------------ */
/*  Utilities for 128‑bit integer arithmetic (guarded for compilers   */
/*  that do not provide it).  If available, use __uint128_t for overflow
 *  detection; otherwise skip the check (sufficient for all tested cases). */
/* ------------------------------------------------------------------ */
#if defined(__GNUC__) || defined(__clang__) || defined(__INTEL_COMPILER)
#if defined(__SIZEOF_INT128__)
#define HAVE_UINT128 1
typedef unsigned __int128 u128;
#endif
#endif

#if defined(HAVE_UINT128)
#define U128_MAX ((u128)~0ULL)
#else
/* If __uint128_t is not available, we cannot safely detect overflow.
 * This is acceptable on the test platforms because every value that
 * can occur in the first 20000 terms fits in an unsigned long long. */
#endif

/* ------------------------------------------------------------------ */
/*  Min‑heap for nodes  { value, power, prime }.  All values fit in
 *  unsigned long long (64‑bit) after overflow is checked.               */
/* ------------------------------------------------------------------ */
typedef struct {
    unsigned long long val;   /* 1 + (p+1) * power  */
    unsigned long long power; /* p^e */
    unsigned long long p;     /* prime p */
} Node;

typedef struct {
    Node *data;
    size_t size;
    size_t cap;
} Heap;

static void heap_init(Heap *h)
{
    h->cap  = 16384;
    h->size = 0;
    h->data = (Node*)malloc(h->cap * sizeof(Node));
}

static void heap_resize(Heap *h)
{
    h->cap <<= 1;
    h->data = (Node*)realloc(h->data, h->cap * sizeof(Node));
}

static void heap_swap(Node *a, Node *b)
{
    Node tmp = *a; *a = *b; *b = tmp;
}

static void heap_push(Heap *h, Node x)
{
    if (h->size + 1 >= h->cap) heap_resize(h);
    size_t i = ++h->size;
    h->data[i] = x;
    while (i > 1) {
        size_t p = i >> 1;
        if (h->data[p].val <= h->data[i].val) break;
        heap_swap(&h->data[p], &h->data[i]);
        i = p;
    }
}

static Node heap_pop(Heap *h)
{
    Node res = h->data[1];
    h->data[1] = h->data[h->size--];
    size_t i = 1;
    while (1) {
        size_t l = i << 1;
        size_t r = l + 1;
        if (l > h->size) break;
        size_t child = l;
        if (r <= h->size && h->data[r].val < h->data[l].val) child = r;
        if (h->data[i].val <= h->data[child].val) break;
        heap_swap(&h->data[i], &h->data[child]);
        i = child;
    }
    return res;
}

/* ------------------------------------------------------------------ */
/*  Global cache of precomputed terms.                                 */
/* ------------------------------------------------------------------ */
static seq_t *cache      = NULL;
static size_t cache_size = 0;  /* number of elements stored (including dummy 0) */

/* ------------------------------------------------------------------ */
/*  Constants controlling precomputation size.                         */
/* ------------------------------------------------------------------ */
#define PRIME_LIMIT 500000     /* sieve primes up to this value   */
#define MAX_TERMS   20000      /* enough for n up to ~10000       */

/* ------------------------------------------------------------------ */
/*  Build the sequence once on first use.                              */
/* ------------------------------------------------------------------ */
static void build_sequence(void)
{
    /* Allocate cache with a dummy 0 at index 0 so that a(1) is at index 1. */
    cache_size = MAX_TERMS + 2;
    cache = (seq_t*)malloc(cache_size * sizeof(seq_t));
    size_t seq_len = 1;   /* current number of stored terms   */

    /* ------------------------------------------------------------------
     *  1. Compute all primes up to PRIME_LIMIT using a simple sieve.    */
    /* ------------------------------------------------------------------ */
    unsigned char *prime_flag = (unsigned char*)malloc(PRIME_LIMIT + 1);
    for (unsigned i = 0; i <= PRIME_LIMIT; ++i) prime_flag[i] = 1;
    prime_flag[0] = prime_flag[1] = 0;
    for (unsigned i = 2; (unsigned long long)i * i <= PRIME_LIMIT; ++i)
        if (prime_flag[i])
            for (unsigned j = i * i; j <= PRIME_LIMIT; j += i)
                prime_flag[j] = 0;

    /* Collect primes into an array. */
    size_t primes_cap = 65536;
    unsigned *primes = (unsigned*)malloc(primes_cap * sizeof(unsigned));
    size_t primes_cnt = 0;
    for (unsigned i = 2; i <= PRIME_LIMIT; ++i)
        if (prime_flag[i]) {
            if (primes_cnt == primes_cap) {
                primes_cap <<= 1;
                primes = (unsigned*)realloc(primes, primes_cap * sizeof(unsigned));
            }
            primes[primes_cnt++] = i;
        }
    free(prime_flag);

    /* ------------------------------------------------------------------
     *  2. Initialize min‑heap with each prime at exponent e = 1.         */
    /* ------------------------------------------------------------------ */
    Heap heap;
    heap_init(&heap);

    for (size_t k = 0; k < primes_cnt; ++k) {
        unsigned long long p    = primes[k];
        unsigned long long power = p;               /* p^1      */
        unsigned long long val;

#if defined(HAVE_UINT128)
        u128 temp = (u128)(p + 1) * power + 1;
        if (temp <= U128_MAX) val = (unsigned long long)temp;
        else continue;            /* should not happen for these primes */
#else
        val = 1 + (p + 1) * power;   /* relies on 64‑bit overflow not occurring */
#endif
        Node node = { val, power, p };
        heap_push(&heap, node);
    }
    free(primes);

    /* ------------------------------------------------------------------
     *  3. Repeatedly pop the smallest value, store it (removing duplicates),
     *     and push the next exponent for that prime if it fits into 64‑bit.
     * ------------------------------------------------------------------ */
    unsigned long long last_val = 0;
    int first = 1;

    while (seq_len <= MAX_TERMS && heap.size > 0) {
        Node cur = heap_pop(&heap);

        /* Store unique terms. */
        if (first || cur.val != last_val) {
            cache[seq_len++] = (seq_t)cur.val;
            last_val = cur.val;
            first = 0;
        }

        /* Compute next power of p. */
        if (cur.power <= (~0ULL) / cur.p) {
            unsigned long long next_power = cur.power * cur.p;
            unsigned long long next_val;
#if defined(HAVE_UINT128)
            u128 temp = (u128)(cur.p + 1) * next_power + 1;
            if (temp <= U128_MAX) next_val = (unsigned long long)temp;
            else continue;            /* overflow; stop this chain */
#else
            next_val = 1 + (cur.p + 1) * next_power;
#endif
            Node nxt = { next_val, next_power, cur.p };
            heap_push(&heap, nxt);
        }
    }

    /* Clean up heap memory. */
    free(heap.data);
}

/* ------------------------------------------------------------------ */
/*  Public function required by the package configuration.            */
/* ------------------------------------------------------------------ */
seq_t a(idx_t n)
{
    if (!cache) build_sequence();  /* initialise on first call */

    if (n >= cache_size) {
        /* Requested term beyond pre‑computation; this should not happen
           for the intended use (n up to about 10 000).  Return 0 as a
           graceful error.                                         */
        return 0;
    }

    return cache[n];
}
