/*
 * A194868  Triangular array (and fractal sequence): row n is the permutation
 *          of (1,2,...,n) obtained from the increasing ordering of fractional
 *          parts {r}, {2r}, ..., {nr}, where r=-(1+sqrt(3))/2.
 * https://oeis.org/A194868
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, tabl
 * Formula:   not stated
 * Range:     u8 n <= 94, u16 n <= 94, u32 n <= 94, u64 n <= 94, u128 n <= 94,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A194868: Triangular array permutation based on fractional parts
   of k*r, where r = -(1+sqrt(3))/2.  The implementation uses integer
   arithmetic with a large modulus to avoid floating‑point errors. */

#include <stdlib.h>
#include <stdint.h>
#include <stddef.h>

/* Integer square root of an unsigned 128‑bit integer. */
static unsigned __int128 isqrt128(unsigned __int128 x)
{
    unsigned __int128 r = 0;
    for (int i = 63; i >= 0; --i) {
        unsigned __int128 bit = (unsigned __int128)1 << i;
        unsigned __int128 trial = r | bit;
        if (trial * trial <= x)
            r = trial;
    }
    return r;
}

/* Node used for sorting each row: the key and the 1‑based index k. */
struct Node {
    idx_t idx;
    unsigned __int128 key;
};

/* Comparator: sort by descending key, then ascending idx. */
static int cmp_node(const void *a, const void *b)
{
    const struct Node *x = (const struct Node *)a;
    const struct Node *y = (const struct Node *)b;
    if (x->key > y->key) return -1;
    if (x->key < y->key) return 1;
    if (x->idx < y->idx) return -1;
    if (x->idx > y->idx) return 1;
    return 0;
}

/* ------------------------------------------------------------------ */
/*  Entry function for the OEIS sequence.                           */
/* ------------------------------------------------------------------ */
seq_t a(idx_t n)
{
    /* Static state for memoisation. */
    static idx_t last_row   = 0;
    static idx_t cache_len  = 0;
    static seq_t *cache      = NULL;
    static size_t cache_cap  = 0;

    /* Constants derived from α = (1+√3)/2. */
    static unsigned __int128 A = 0;      /* numerator ≈ α * 2^62 */
    static unsigned __int128 twoQ = 0;   /* = 2^63 */
    static int initialized = 0;

    if (!initialized) {
        unsigned __int128 Q = ((unsigned __int128)1) << 62;   /* 2^62 */
        unsigned __int128 P = isqrt128((unsigned __int128)3 * Q * Q); /* floor(√3 * Q) */
        A = Q + P;           /* A ≈ (1+√3) * Q */
        twoQ = 2 * Q;        /* twoQ = 2^63 */
        initialized = 1;
    }

    if (n <= cache_len)
        return cache[n - 1];

    idx_t needed_entries = n * (n + 1) / 2;
    if (cache_cap < (size_t)needed_entries) {
        seq_t *new_cache = (seq_t *)malloc(sizeof(seq_t) * needed_entries);
        if (!new_cache) abort();
        if (cache) {
            for (idx_t i = 0; i < cache_len; ++i)
                new_cache[i] = cache[i];
            free(cache);
        }
        cache = new_cache;
        cache_cap = (size_t)needed_entries;
    }

    for (idx_t r = last_row + 1; r <= n; ++r) {
        struct Node *nodes = (struct Node *)malloc(sizeof(struct Node) * r);
        if (!nodes) abort();

        for (idx_t k = 1; k <= r; ++k) {
            unsigned __int128 prod = (unsigned __int128)k * A;
            nodes[k - 1].idx = k;
            nodes[k - 1].key = prod % twoQ;
        }

        qsort(nodes, r, sizeof(struct Node), cmp_node);

        for (idx_t i = 0; i < r; ++i)
            cache[cache_len++] = (seq_t)nodes[i].idx;

        free(nodes);
    }

    last_row = n;
    return cache[n - 1];
}
