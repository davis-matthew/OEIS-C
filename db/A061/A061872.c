/*
 * A061872  |First digit - second digit + third digit - fourth digit ...| = 3.
 * https://oeis.org/A061872
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base, easy
 * Formula:   not stated
 * Range:     u8 n <= 34, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  Sequence A061872
 *  a(n) = the n-th positive integer whose alternating sum of decimal
 *         digits (first minus second plus third …) has absolute value 3.
 *  Example: 3, 14, 25, 30, 36, 41, …   (given initial terms)
 */

#include <stdlib.h>

/* Return the n-th term of A061872.
 * The function keeps a static cache so that multiple calls are fast
 * regardless of the order of n.
 */
seq_t a(idx_t n)
{
    /* static cache of previously computed terms   */
    static seq_t *cache = NULL;
    static idx_t cache_len = 0;      /* number of terms currently stored */
    static idx_t test_value = 1;     /* next integer to test for the property */

    /* if already computed, return result directly */
    if (n <= cache_len)
        return cache[n - 1];

    /* Grow cache to hold the needed number of terms */
    seq_t *new_cache = (seq_t *)realloc(cache, sizeof(seq_t) * n);
    if (!new_cache) abort();          /* out of memory */
    cache = new_cache;

    /* Find new terms until we have n of them */
    while (cache_len < n) {
        idx_t m = test_value;
        int sum = 0;
        int sign = 1;                 /* +1 for the first digit, then alternates */

        /* Compute alternating sum of digits */
        while (m) {
            sum += sign * (int)(m % 10);
            m /= 10;
            sign = -sign;
        }

        if (abs(sum) == 3) {          /* satisfies definition */
            cache[cache_len] = (seq_t)test_value;
            ++cache_len;
        }
        ++test_value;                /* advance to next candidate */
    }

    return cache[n - 1];
}
