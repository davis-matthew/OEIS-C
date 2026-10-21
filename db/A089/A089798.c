/*
 * A089798  Expansion of Jacobi theta function theta_4(q^2).
 * https://oeis.org/A089798
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign
 * Formula:   a(n) = 2*(floor(sqrt(n/2)) -
 *            floor(sqrt((n-1)/2)))*(-1)^floor(sqrt(n/2)) * with a(0) = 1.
 *            Computes the OEIS sequence A089798
 * Range:     i8 n <= 4999, i16 n <= 5000, i32 n <= 5000, i64 n <= 5000,
 *            i128 n <= 5000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

static seq_t floor_sqrt_seq(seq_t x)
{
    if (x == 0 || x == 1)
        return x;

    /* Highest possible floor(sqrt(max)) is 2^(w/2)-1, where w is bit width of seq_t. */
    seq_t high = ((seq_t)1 << ((sizeof(seq_t) * 8) / 2)) - 1;
    seq_t low  = 0;

    while (low < high) {
        seq_t mid = (low + high + 1) >> 1;
        seq_t sq  = mid * mid;
        if (sq <= x)
            low = mid;
        else
            high = mid - 1;
    }
    return low;
}

seq_t a(idx_t n)
{
    if (n == 0)          /* a(0) = 1 */
        return 1;

    seq_t m  = n >> 1;             /* floor(n/2)          */
    seq_t km = floor_sqrt_seq(m);   /* floor(sqrt(floor(n/2))) */

    seq_t m2  = (n - 1) >> 1;          /* floor((n-1)/2)     */
    seq_t k2  = floor_sqrt_seq(m2);    /* floor(sqrt(floor((n-1)/2))) */

    seq_t diff = km - k2;              /* 0 or 1               */
    seq_t val  = diff * ((seq_t)2);    /* 0 or 2               */

    if (km & 1)                         /* odd k => negative */
        val = -val;

    return val;
}
