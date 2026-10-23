/*
 * A179865  Number of n-bit binary numbers containing one run of 0's.
 * https://oeis.org/A179865
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base, easy
 * Formula:   a(1) = 1; for n >= 2, a(n) = (n-1)*n/2
 * Range:     u8 n <= 23, u16 n <= 362, u32 n <= 500, u64 n <= 500,
 *            u128 n <= 500, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n <= 1)
        return 1;                 /* a(1) */

    seq_t nn = (seq_t)n;           /* safe cast to seq_t */
    return (nn - 1) * nn / 2;      /* (n-1)*n/2 */
}
