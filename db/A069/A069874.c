/*
 * A069874  a(1) = a(2) = 1; a(n) = 2^a(n-1) + 2^a(n-2) for n > 2.
 * https://oeis.org/A069874
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(1) = a(2) = 1; a(n) = 2^a(n-1) + 2^a(n-2) for n > 2
 * Range:     u8 n <= 4, u16 n <= 4, u32 n <= 5, u64 n <= 5, u128 n <= 5,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

/* Cached sequence values */
static seq_t *cache = NULL;   /* cache[0] is unused, cache[1] = 1, cache[2] = 1 */
static idx_t cache_len = 0;   /* last index stored in cache */

/* Compute 2^exp using exponentiation by squaring.
   All operations are on the generic seq_t type. */
static seq_t pow2_exp(seq_t exp) {
    seq_t result = 1;   /* 2^0 */
    seq_t base   = 2;   /* base = 2 */

    while (exp != 0) {
        /* if lowest bit of exponent is set, multiply current result by base */
        if ((exp & 1) != 0) {
            result = result * base;
        }
        /* shift exponent right by one (divide by 2) */
        exp = exp / 2;
        /* square the base for next bit */
        base = base * base;
    }
    return result;
}

/* Return a(n) according to the recurrence. */
seq_t a(idx_t n) {
    if (n == 0) return 0;   /* undefined but harmless */

    /* Initialise cache lazily */
    if (cache == NULL) {
        cache_len = 2;
        cache = (seq_t*)malloc(sizeof(seq_t) * 3);   /* indices 0..2 */
        cache[0] = 0;
        cache[1] = 1;
        cache[2] = 1;
    }

    /* Extend cache if we need a higher index */
    if (n > cache_len) {
        size_t new_size = (size_t)n + 1;             /* enough for indices 0..n */
        cache = (seq_t*)realloc(cache, new_size * sizeof(seq_t));
        for (idx_t i = cache_len + 1; i <= n; ++i) {
            seq_t term = pow2_exp(cache[i-1]) + pow2_exp(cache[i-2]);
            cache[i] = term;
        }
        cache_len = n;
    }

    return cache[n];
}
