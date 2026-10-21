/*
 * A091033  Third column (k=4) of array A090438 ((4,2)-Stirling2).
 * https://oeis.org/A091033
 *
 * Offset:    2    (a(2) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = (n-1)*(2*n-3)*(2*n)!/4! for n>=2 Recurrence: a(n) =
 *            a(n-1)*((n-1)*(2*n-3)*(2*n)*(2*n-1))/((n-2)*(2*n-5))
 * Range:     u8 n <= 3, u16 n <= 4, u32 n <= 6, u64 n <= 10, u128 n <= 13,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>

seq_t a(idx_t n)
{
    if (n < 2) return 0;          /* offset not defined below 2 */

    /* Cached previously computed terms */
    static seq_t *cache = NULL;
    static idx_t max_cached = 1; /* the highest n for which cache[n] is valid */

    /* Initialise cache with a(2)=1 */
    if (cache == NULL) {
        cache = (seq_t *)malloc(sizeof(seq_t) * 3);
        cache[0] = 0;               /* unused */
        cache[1] = 0;               /* unused */
        cache[2] = (seq_t)1;        /* a(2) */
        max_cached = 2;
    }

    /* Return already known value */
    if (n <= max_cached) return cache[n];

    /* Extend cache array up to n */
    cache = (seq_t *)realloc(cache, sizeof(seq_t) * (n + 1));

    /* Compute missing terms using the recurrence */
    for (idx_t i = max_cached + 1; i <= n; ++i) {
        seq_t prev = cache[i - 1];

        /* Build numerator and denominator of the multiplicative factor
           factor = ((i-1)*(2*i-3)*(2*i)*(2*i-1))/((i-2)*(2*i-5)) */
        idx_t num1 = (i - 1);
        idx_t num2 = (2 * i - 3);
        idx_t num3 = (2 * i);
        idx_t num4 = (2 * i - 1);
        idx_t den1 = (i - 2);
        idx_t den2 = (2 * i - 5);

        /* Use 128‑bit intermediate for the 64‑bit build to avoid overflow.
           For big‑integer builds the normal seq_t arithmetic suffices. */
        if (sizeof(seq_t) <= 8) {
            unsigned __int128 mult = (unsigned __int128)prev * num1;
            mult = mult * num2;
            mult = mult * num3;
            mult = mult * num4;
            unsigned __int128 denom = (unsigned __int128)den1 * den2;
            cache[i] = (seq_t)(mult / denom);
        } else {
            /* seq_t can hold arbitrary precision */
            seq_t mult = prev * (seq_t)num1;
            mult = mult * (seq_t)num2;
            mult = mult * (seq_t)num3;
            mult = mult * (seq_t)num4;
            seq_t denom = (seq_t)den1 * (seq_t)den2;
            cache[i] = mult / denom;
        }
    }

    max_cached = n;
    return cache[n];
}
