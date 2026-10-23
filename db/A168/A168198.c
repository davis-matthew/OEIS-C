/*
 * A168198  a(n) = 3*n - a(n-1) + 1 with n > 1, a(1)=1.
 * https://oeis.org/A168198
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = (6*n + 5 + 7*(-1)^n)/4
 * Range:     u8 n <= 169, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Equivalent integer formulas: */
/*   if n is odd:  a(n) = (3*n - 1)/2 */
/*   if n is even: a(n) = (3*n + 6)/2 */

seq_t a(idx_t n)
{
    seq_t nn = (seq_t)n;                /* cast index to seq_t */
    if ((n & 1) == 0) {                /* even n */
        return ((seq_t)3 * nn + 6) / 2;
    } else {                           /* odd n */
        return ((seq_t)3 * nn - 1) / 2;
    }
}
