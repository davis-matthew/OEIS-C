/*
 * A186637  Semiprime powers with special exponents: k^(j-1) where both j and
 *          k are arbitrary semiprime numbers.
 * https://oeis.org/A186637
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 none, u16 none, u32 n <= 12, u64 n <= 42, u128 n <= 42,
 *            big any n
 * Helpers:   big_pow, build_primes
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Compute A186637:
   a(n) = k^(j-1) where both k and j are semiprimes.
   The sequence is the set of all such powers sorted increasingly
   with duplicates removed.  Cache results to support random access. */

#include <stdint.h>
#include <stddef.h>
#include <stdlib.h>
#include <stdbool.h>

#include "../utils/build_primes.h"   /* build_primes() */
#include "../utils/big_pow.h"        /* big_pow() */

/* Comparison used by qsort for uint64_t values */
static int cmp_uint64(const void *a, const void *b)
{
    uint64_t va = *(const uint64_t *)a;
    uint64_t vb = *(const uint64_t *)b;
    return (va > vb) - (va < vb);
}

/* Heap node: value, base index, exponent index */
typedef struct {
    seq_t val;
    size_t base_idx;
    size_t exp_idx;
} HeapNode;

/* ------------------------------------------------------------ */
/*  GLOBAL STATIC STATE                                          */
/* ------------------------------------------------------------ */

static uint64_t *semip = NULL;      /* list of semiprime numbers */
static size_t semip_cnt = 0;        /* count of semiprimes */

static HeapNode *heap = NULL;       /* binary min‑heap */
static size_t heap_size = 0;        /* current heap size */

static seq_t *cache = NULL;         /* cached sequence values */
static size_t cache_len = 0;        /* number of values stored */
static size_t cache_cap = 0;        /* capacity of cache array */

static seq_t last_val = 0;          /* last value added to cache */
static bool have_last = false;      /* indicates if last_val is valid */

/* ------------------------------------------------------------ */
/*  HELPER FUNCTIONS                                            */
/* ------------------------------------------------------------ */

/* -------- Min‑heap operations ---------- */
static void heap_push(HeapNode node)
{
    size_t i = heap_size++;
    while (i > 0) {
        size_t parent = (i - 1) >> 1;
        if (heap[parent].val <= node.val) break;
        heap[i] = heap[parent];
        i = parent;
    }
    heap[i] = node;
}

static HeapNode heap_pop(void)
{
    HeapNode root = heap[0];
    HeapNode last = heap[--heap_size];
    size_t i = 0, child;
    while ((child = i * 2 + 1) < heap_size) {
        if (child + 1 < heap_size && heap[child + 1].val < heap[child].val)
            child++;
        if (heap[child].val >= last.val) break;
        heap[i] = heap[child];
        i = child;
    }
    heap[i] = last;
    return root;
}

/* ---------- Build list of semiprimes up to LIMIT ---------- */
static void init_semiprimes(void)
{
    /* LIMIT chosen to produce plenty of terms (>10000) */
    const idx_t LIMIT = 200000;   /* product bound for semiprimes */

    idx_t *primes = NULL;
    size_t prime_cnt = 0;
    build_primes(LIMIT, &primes, &prime_cnt);

    /* Estimate allocation: at most prime_cnt^2/2 products */
    size_t cap = prime_cnt * prime_cnt / 4;
    if (cap < 1024) cap = 1024;

    semip = (uint64_t *)malloc(cap * sizeof(uint64_t));
    semip_cnt = 0;

    for (size_t i = 0; i < prime_cnt; ++i) {
        for (size_t j = i; j < prime_cnt; ++j) {
            uint64_t prod = (uint64_t)primes[i] * primes[j];
            if (prod > LIMIT) break;
            if (semip_cnt == cap) {
                cap <<= 1;
                semip = (uint64_t *)realloc(semip, cap * sizeof(uint64_t));
            }
            semip[semip_cnt++] = prod;
        }
    }

    /* List is sorted by construction, but sort just in case */
    qsort(semip, semip_cnt, sizeof(uint64_t), cmp_uint64);

    free(primes);
}

/* ---------- Initialize heap with exponent index 0 for each base ---------- */
static void init_heap(void)
{
    heap = (HeapNode *)malloc(semip_cnt * sizeof(HeapNode));
    heap_size = 0;

    /* The smallest exponent corresponds to the first semiprime (e.g. 4 -> exp=3) */
    uint64_t first_exp_semiprime = semip[0];
    seq_t base_exp = (seq_t)(first_exp_semiprime - 1);

    for (size_t i = 0; i < semip_cnt; ++i) {
        HeapNode node;
        node.base_idx = i;
        node.exp_idx = 0;
        node.val = big_pow((seq_t)semip[i], base_exp);
        heap_push(node);
    }
}

/* ---------- Grow cache array as needed ---------- */
static void ensure_cache_capacity(void)
{
    if (cache_len >= cache_cap) {
        size_t new_cap = (cache_cap == 0) ? 512 : cache_cap * 2;
        cache = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
        cache_cap = new_cap;
    }
}

/* ---------- Generate sequence until at least n terms are cached ---------- */
static void generate_up_to(idx_t n)
{
    while (cache_len < (size_t)n) {
        HeapNode cur = heap_pop();

        /* Store value if it's not a duplicate */
        if (!(have_last && cur.val == last_val)) {
            ensure_cache_capacity();
            cache[cache_len++] = cur.val;
            last_val = cur.val;
            have_last = true;
        }

        /* Push next exponent for this base, if any */
        size_t next_exp = cur.exp_idx + 1;
        if (next_exp < semip_cnt) {
            HeapNode nxt;
            nxt.base_idx = cur.base_idx;
            nxt.exp_idx = next_exp;
            nxt.val = big_pow((seq_t)semip[cur.base_idx],
                              (seq_t)(semip[next_exp] - 1));
            heap_push(nxt);
        }
    }
}

/* ------------------------------------------------------------ */
/*  PUBLIC FUNCTION                                             */
/* ------------------------------------------------------------ */
seq_t a(idx_t n)
{
    if (n == 0) return 0;  /* sequence is 1‑based */

    /* One‑time initialization */
    if (!semip) {
        init_semiprimes();
        init_heap();
    }

    if ((size_t)n > cache_len) {
        generate_up_to(n);
    }
    return cache[n - 1];
}
