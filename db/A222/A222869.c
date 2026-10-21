/*
 * A222869  Number of n X 3 0..5 arrays with no element equal to another at a
 *          city block distance of exactly two, and new values 0..5 introduced
 *          in row major order.
 * https://oeis.org/A222869
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 42*a(n‑1) – 117*a(n‑2) for n>4, a(1)=3, a(2)=34,
 *            a(3)=816, a(4)=30528
 * Range:     u8 n <= 2, u16 n <= 4, u32 n <= 7, u64 n <= 13, u128 n <= 25,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n)
{
    /* we keep a cache of previously computed terms */
    static seq_t *cache = NULL;     /* array of values */
    static idx_t  computed = 0;     /* largest n that has been computed   */
    static size_t capacity = 0;     /* size of allocated array (in entries) */

    if (n < 1)   /* sequence is defined only for n≥1 */
        return (seq_t)0;

    /* initialise: make sure we have room for the first 4 values (indices 1‑4) */
    if (cache == NULL) {
        capacity = 5;                       /* indices 0..4 are used */
        cache = (seq_t *)malloc(capacity * sizeof(seq_t));
        if (cache == NULL) return (seq_t)0;  /* allocation failure – abort */
    }

    /* first time: populate the known initial values */
    if (computed == 0) {
        cache[0] = (seq_t)0;               /* unused slot – keeps indices 1‑based */
        cache[1] = (seq_t)3;               /* a(1) */
        cache[2] = (seq_t)34;              /* a(2) */
        cache[3] = (seq_t)816;             /* a(3) */
        cache[4] = (seq_t)30528;           /* a(4) */
        computed = 4;
    }

    /* if the requested n hasn't been computed yet, compute up to it */
    if (n > computed) {
        /* grow the cache if necessary (simple doubling strategy) */
        while (capacity <= n) {
            size_t new_cap = capacity * 2;
            if (new_cap == 0) new_cap = 128;      /* safety for very small start */
            seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
            if (tmp == NULL) return (seq_t)0;     /* allocation failure – abort */
            cache = tmp;
            capacity = new_cap;
        }

        /* compute terms using the recurrence */
        for (idx_t i = computed + 1; i <= n; ++i) {
            cache[i] = ((seq_t)42) * cache[i - 1] - ((seq_t)117) * cache[i - 2];
        }
        computed = n;
    }

    return cache[n];
}
