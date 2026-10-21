/*
 * A077432  Squares of the form u'v'w, where in decimal representation u=n^2,
 *          v=2*n^2 and w=n^2 possibly with a leading zero.
 * https://oeis.org/A077432
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base, nice
 * Formula:   a(n) = ( n * ( 1 + 10^digits_of( 2 * n^2 ) ) )^2
 * Range:     u8 n <= 1, u16 n <= 2, u32 n <= 22, u64 n <= 23, u128 n <= 23,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* where digits_of(x) is the number of decimal digits of x. */
#include <stdbool.h>

seq_t a(idx_t n)
{
    /* Convert n to seq_t once */
    seq_t sn = (seq_t)n;

    /* Compute m = 2 * n^2 */
    seq_t m = sn * sn * (seq_t)2;

    /* Count decimal digits of m */
    int digits = 0;
    seq_t tmp = m;
    while (tmp != (seq_t)0) {
        tmp /= (seq_t)10;
        ++digits;
    }
    if (digits == 0) digits = 1;  /* for the case n=0, not expected */

    /* Compute 10^digits as seq_t */
    seq_t tenpow = (seq_t)1;
    for (int i = 0; i < digits; ++i)
        tenpow *= (seq_t)10;

    /* Inner term n * (1 + 10^digits) */
    seq_t inner = sn * ((seq_t)1 + tenpow);

    /* a(n) = inner^2 */
    return inner * inner;
}
