/*
 * A175131  a(n) = 2^(n-1) mod semiprime(n).
 * https://oeis.org/A175131
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, less
 * Formula:   a(n) = 2^(n-1) mod semiprime(n)
 * Range:     u8 none, u16 none, u32 n <= 1000, u64 n <= 1000, u128 n <= 1000,
 *            big any n
 * Helpers:   build_primes_upto, mod_pow, seq_alloc_array
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stddef.h>
#include <stdbool.h>
#include <stdlib.h>

/* Utilities that are available in the build environment */
#include "../utils/build_primes_upto.h"
#include "../utils/mod_pow.h"
#include "../utils/seq_alloc_array.h"

struct Node {
    seq_t val;      /* product p[i] * p[j]   */
    size_t i;       /* prime index i         */
    size_t j;       /* prime index j >= i    */
};

#ifdef __cplusplus
/* ----------- C++ implementation using std::priority_queue ----------- */
#include <queue>
static void generate_semiprimes(size_t needed, seq_t *out,
                                uint64_t *primes, size_t prime_cnt)
{
    struct Compare {
        bool operator()(const struct Node &a, const struct Node &b) const {
            return a.val > b.val;          /* min‑heap */
        }
    };
    std::priority_queue<struct Node,
        std::vector<struct Node>,
        Compare> pq;

    for (size_t i = 0; i < prime_cnt; ++i) {
        pq.push({ (seq_t)primes[i] * (seq_t)primes[i], i, i });
    }

    size_t produced = 0;
    while (produced < needed && !pq.empty()) {
        struct Node cur = pq.top(); pq.pop();
        out[produced++] = cur.val;
        if (cur.j + 1 < prime_cnt) {
            pq.push({ (seq_t)primes[cur.i] * (seq_t)primes[cur.j + 1],
                      cur.i, cur.j + 1 });
        }
    }
}

/* ----------- C implementation with a hand‑rolled binary heap ----------- */
#else
struct Heap {
    struct Node *data;
    size_t size;
    size_t cap;
};

static void heap_init(struct Heap *h, size_t cap)
{
    h->data = (struct Node*)malloc(cap * sizeof(struct Node));
    h->size = 0;
    h->cap  = cap;
}

static void heap_destroy(struct Heap *h)
{
    free(h->data);
    h->size = h->cap = 0;
}

static void heap_swap(struct Node *a, struct Node *b)
{
    struct Node tmp = *a;
    *a = *b;
    *b = tmp;
}

static void heap_push(struct Heap *h, struct Node node)
{
    if (h->size == h->cap) {
        h->cap *= 2;
        h->data = (struct Node*)realloc(h->data,
                                        h->cap * sizeof(struct Node));
    }
    h->data[h->size++] = node;
    size_t i = h->size - 1;
    while (i > 0) {
        size_t p = (i - 1) >> 1;
        if (h->data[p].val <= h->data[i].val) break;
        heap_swap(&h->data[p], &h->data[i]);
        i = p;
    }
}

static struct Node heap_pop(struct Heap *h)
{
    struct Node res = h->data[0];
    h->data[0] = h->data[--h->size];
    size_t i = 0, left, right, smallest;
    while (true) {
        left  = (i << 1) + 1;
        right = (i << 1) + 2;
        smallest = i;
        if (left < h->size && h->data[left].val < h->data[smallest].val)
            smallest = left;
        if (right < h->size && h->data[right].val < h->data[smallest].val)
            smallest = right;
        if (smallest == i) break;
        heap_swap(&h->data[i], &h->data[smallest]);
        i = smallest;
    }
    return res;
}

static void generate_semiprimes(size_t needed, seq_t *out,
                                uint64_t *primes, size_t prime_cnt)
{
    struct Heap h;
    heap_init(&h, prime_cnt);

    for (size_t i = 0; i < prime_cnt; ++i) {
        struct Node n = { (seq_t)primes[i] * (seq_t)primes[i], i, i };
        heap_push(&h, n);
    }

    size_t produced = 0;
    while (produced < needed && h.size > 0) {
        struct Node cur = heap_pop(&h);
        out[produced++] = cur.val;
        if (cur.j + 1 < prime_cnt) {
            struct Node nxt = { (seq_t)primes[cur.i] * (seq_t)primes[cur.j + 1],
                                cur.i, cur.j + 1 };
            heap_push(&h, nxt);
        }
    }
    heap_destroy(&h);
}
#endif

/* ------------------- Main sequence function ------------------- */
seq_t
a(idx_t n)
{
    /* Static state – memoised between calls */
    static seq_t          *semiprimes  = NULL;
    static size_t          semiprime_cap   = 0;
    static size_t          semiprime_cnt   = 0;
    static uint64_t        *primes     = NULL;
    static size_t          prime_cnt   = 0;
    static uint64_t        prime_limit = 0;

    /* Initialise once */
    if (semiprimes == NULL) {
        /* Initial prime limit – large enough for the first 10 000 terms */
        prime_limit = 200000;
        build_primes_upto(prime_limit, &primes, &prime_cnt);

        /* Allocate space for semiprimes */
        semiprime_cap = 25000;                         /* extra room */
        semiprimes = (seq_t*)malloc(semiprime_cap *
                                    sizeof(seq_t));

        /* Generate the first semiprime_cap terms */
        generate_semiprimes(semiprime_cap, semiprimes,
                            primes, prime_cnt);
        semiprime_cnt = semiprime_cap;
    }

    /* Expand the cache if necessary */
    if (n > semiprime_cnt) {
        size_t new_cap = semiprime_cnt;
        while (new_cap < (size_t)n) new_cap <<= 1;
        semiprimes = (seq_t*)realloc(semiprimes,
                                     new_cap * sizeof(seq_t));

        /* If the existing prime list is too small, extend it */
        if (primes && primes[prime_cnt-1] < n) {
            prime_limit *= 2;
            free(primes);
            build_primes_upto(prime_limit, &primes, &prime_cnt);
        }

        generate_semiprimes(new_cap, semiprimes, primes, prime_cnt);
        semiprime_cnt = new_cap;
    }

    /* The n-th semiprime (offset 1) */
    seq_t mod = semiprimes[n-1];

    /* 2^(n-1) mod mod */
    return mod_pow((seq_t)2, (uint64_t)(n-1), mod);
}
