/*
 * A041229  Denominators of continued fraction convergents to sqrt(126).
 * https://oeis.org/A041229
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, frac, easy
 * Formula:   a(n) = 898*a(n-4) - a(n-8); Initial values: a(0)=1, a(1)=4,
 *            a(2)=9, a(3)=40, a(4)=889, a(5)=3596, a(6)=8081, a(7)=35920
 * Range:     u8 n <= 3, u16 n <= 7, u32 n <= 13, u64 n <= 26, u128 n <= 52,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

static seq_t *cache = NULL;      /* dynamic array of computed terms */
static size_t cap   = 0;         /* current capacity of cache */
static size_t size  = 0;         /* number of terms stored (highest index + 1) */

/* first eight terms from the OEIS entry */
static const seq_t init[8] = {
    ((seq_t)1),
    ((seq_t)4),
    ((seq_t)9),
    ((seq_t)40),
    ((seq_t)889),
    ((seq_t)3596),
    ((seq_t)8081),
    ((seq_t)35920)
};

/* enlarge cache so that it can hold indices 0 .. new_cap-1 */
static void ensure_cache_size(size_t new_cap) {
    if (new_cap <= cap) return;
    size_t new_c = (cap == 0) ? 16 : cap;
    while (new_c < new_cap) new_c <<= 1;
    seq_t *tmp = (seq_t *)realloc(cache, new_c * sizeof(seq_t));
    if (!tmp) abort();            /* allocation failure – terminate */
    cache = tmp;
    cap = new_c;
}

seq_t a(idx_t n) {
    /* trivial small indices */
    if (n < 8) return init[(size_t)n];

    /* initialise cache on first call */
    if (!cache) {
        cap = 16;
        cache = (seq_t *)malloc(cap * sizeof(seq_t));
        for (size_t i = 0; i < 8; ++i) cache[i] = init[i];
        size = 8;                 /* stored indices 0..7 */
    }

    /* already computed? */
    if ((size_t)n < size) return cache[(size_t)n];

    /* need to compute up to n */
    ensure_cache_size((size_t)n + 1);
    for (size_t i = size; i <= (size_t)n; ++i) {
        /* recurrence: a(i) = 898*a(i-4) - a(i-8) */
        seq_t val = 898 * cache[i - 4] - cache[i - 8];
        cache[i] = val;
    }
    size = (size_t)n + 1;
    return cache[(size_t)n];
}
