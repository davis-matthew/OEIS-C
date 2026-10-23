/*
 * A170673  Number of reduced words of length n in Coxeter group on 40
 *          generators S_i with relations (S_i)^2 = (S_i S_j)^49 = I.
 * https://oeis.org/A170673
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 1, u16 n <= 3, u32 n <= 6, u64 n <= 12, u128 n <= 24,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  OEIS A170673
 *
 *  Recurrence definition
 *    a(0) = 1
 *    a(n) = 38 · (a(n-1)+a(n-2)+…+a(n-48))
 *             – 741 · a(n-49) + p(n)   for n ≥ 0
 *
 *  where the inhomogeneous term p(n) is
 *    p(0) = 1,
 *    p(1)…p(48) = 2,
 *    p(49)      = 1,
 *    p(n) = 0   for n > 49.
 *
 *  The function below memoises the values in a static array and
 *  evaluates the recurrence lazily.  It is valid both as C11 and
 *  C++17.
 */

#include <stdlib.h>

/*  External integer types supplied by the build system  */
extern seq_t a(idx_t n);

/*  Compute a(n) according to the recurrence above  */
seq_t a(idx_t n)
{
    /* static cache: array of previously computed a(i) values   */
    static seq_t *cache   = NULL;   /* cache[0..cache_len-1] contains a(0)..a(cache_len-1) */
    static idx_t   cache_len = 0;    /* current number of stored values          */

    /* Allocate/resize cache if necessary   */
    if (n + 1 > cache_len) {
        /* use realloc with explicit cast so it compiles in both C and C++ */
        seq_t *new_cache = (seq_t *)realloc(cache, sizeof(seq_t) * (n + 1));
        if (!new_cache) abort();          /* out of memory – terminate */
        cache = new_cache;

        /* Compute terms from the previously stored index up to n   */
        for (idx_t i = cache_len; i <= n; ++i) {
            seq_t sum48 = 0;
            /* sum of a(i-1) … a(i-48); ignore indices < 0         */
            for (idx_t j = 1; j <= 48 && i >= j; ++j)
                sum48 += cache[i - j];

            /* p(i) from the numerator of the rational G.F.        */
            seq_t p;
            if (i == 0)           p = 1;
            else if (i <= 48)     p = 2;
            else if (i == 49)     p = 1;
            else                  p = 0;

            /* recurrence */
            seq_t term;
            if (i >= 49)
                term = 38 * sum48 - 741 * cache[i - 49] + p;
            else
                term = 38 * sum48 + p;       /* cache[i-49] does not exist here */

            cache[i] = term;
        }
        cache_len = n + 1;
    }
    return cache[n];
}
