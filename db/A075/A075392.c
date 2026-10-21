/*
 * A075392  Duplicate of A069901.
 * https://oeis.org/A075392
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  dead
 * Formula:   a(n) = least prime factor of n*(n+1)/2 (the n-th triangular
 *            number)
 * Range:     i8 n <= 15, i16 n <= 35, i32 n <= 35, i64 n <= 35, i128 n <= 35,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

seq_t a(idx_t n)
{
    /* Triangular number T(n) = n*(n+1)/2 */
    if (n < 1) return (seq_t)1;          /* safeguard for n < 1 */
    seq_t tn = (seq_t)n * ((seq_t)n + 1) / 2;

    if (tn <= 1) return (seq_t)1;        /* a(1) = 1 */

    /* If T(n) is even, the smallest prime factor is 2 */
    if ((tn & 1) == 0) return (seq_t)2;

    /* For odd T(n), test odd divisors up to sqrt(tn) */
    for (seq_t d = 3; d <= tn / d; d += 2) {
        if (tn % d == 0) return d;
    }

    /* tn itself is prime */
    return tn;
}
