/*
 * A069511  Numbers in which starting from most significant digit the n-th
 *          digit is obtained by adding n to the (n-1)-st digit (the digit to
 *          the left of it) and then ignoring the carry. Alternately the n-th
 *          digit starting from the most significant digit is the n-th
 *          triangular number mod 10.
 * https://oeis.org/A069511
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   not stated
 * Range:     u8 n <= 3, u16 n <= 5, u32 n <= 10, u64 n <= 20, u128 n <= 20,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Recurrence: a(0)=0; a(n)=10*a(n-1)+((n*(n+1)/2) mod 10) */
#include <stdlib.h>

seq_t a(idx_t n)
{
    /* Cache for previously computed terms.  cache[i] holds a(i+1). */
    static seq_t *cache = NULL;      /* pointer to cache array */
    static idx_t cached   = 0;       /* number of terms stored in cache */
    static seq_t lastA    = 0;       /* a(cached) */
    static unsigned int lastTriMod = 0; /* triangular number mod 10 for index = cached */

    if (n == 0)  /* a(0) is defined but not used in offset 1 */
        return 0;

    if (n > cached) {
        idx_t new_len = n;
        /* Grow the cache to hold n terms */
        cache = (seq_t *)realloc(cache, (size_t)new_len * sizeof(seq_t));

        /* Compute new terms from cached+1 up to n */
        for (idx_t i = cached + 1; i <= n; ++i) {
            /* T_i = T_{i-1} + i  (triangular numbers) */
            lastTriMod = (lastTriMod + (unsigned int)i) % 10;
            lastA = lastA * 10 + lastTriMod;
            cache[i - 1] = lastA;   /* store a(i) */
        }
        cached = new_len;
    }
    return cache[n - 1];
}
