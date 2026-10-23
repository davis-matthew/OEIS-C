/*
 * A107417  a(n) = binomial(n+2,2)*binomial(n+5,5).
 * https://oeis.org/A107417
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = binomial(n+2,2) * binomial(n+5,5)
 * Range:     u8 n <= 2, u16 n <= 8, u32 n <= 31, u64 n <= 31, u128 n <= 31,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>  /* for bool, not actually used but required by guidelines */

seq_t a(idx_t n)
{
    /* Compute C(n+2, 2) = (n+2)*(n+1)/2  */
    seq_t c1 = (seq_t)(n + 2) * (n + 1) / 2;

    /* Compute C(n+5, 5) = (n+5)*(n+4)*(n+3)*(n+2)*(n+1)/120  */
    seq_t c2 = (seq_t)(n + 5) * (n + 4) * (n + 3) * (n + 2) * (n + 1) / 120;

    return c1 * c2;
}
