/*
 * A132013  T(n,j) for an iterated mixed order Laguerre transform.
 *          Coefficients of the normalized generalized Laguerre polynomials
 *          (-1)^n*n!*L(n,1-n,x).
 * https://oeis.org/A132013
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, sign, tabl
 * Formula:   a(n) = { 1, if n = k(k+3)/2 for some k ≥ 0; -k, if n =
 *            (k²+3k-2)/2 for some k ≥ 1; 0, otherwise. }
 * Range:     i8 n <= 5049, i16 n <= 5049, i32 n <= 5049, i64 n <= 5049,
 *            i128 n <= 5049, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <math.h>

seq_t a(idx_t n)
{
    if (n == 0)
        return (seq_t)1;               /* n = 0 corresponds to k = 0, a(0) = 1 */

    /* Check for the positive entries: n = k(k+3)/2 */
    unsigned long long d1 = 9ULL + 8ULL * (unsigned long long)n;
    unsigned long long s1 = (unsigned long long)(sqrt((long double)d1) + 0.5L);
    if (s1 * s1 == d1 && s1 >= 3ULL && ((s1 - 3ULL) % 2ULL == 0ULL))
        return (seq_t)1;

    /* Check for the negative entries: n = (k²+3k-2)/2 */
    unsigned long long d2 = 8ULL * (unsigned long long)n + 17ULL;
    unsigned long long s2 = (unsigned long long)(sqrt((long double)d2) + 0.5L);
    if (s2 * s2 == d2 && s2 >= 3ULL && ((s2 - 3ULL) % 2ULL == 0ULL)) {
        unsigned long long k = (s2 - 3ULL) / 2ULL;
        return -(seq_t)k;
    }

    return (seq_t)0;   /* all other indices produce zero */
}
