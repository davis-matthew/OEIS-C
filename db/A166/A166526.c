/*
 * A166526  a(n) = 12*n - a(n-1), with n>1, a(1)=1.
 * https://oeis.org/A166526
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 6*n + 3 + 8*(-1)^n
 * Range:     u8 n <= 41, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* Offset 1 – a(1) is the first listed term. */
    if (n == 0)
        return 0;   /* unused, but keeps the function safe for n==0 */

    seq_t val = (seq_t)6 * n + (seq_t)3;

    /* Add or subtract 8 depending on the parity of n. */
    if ((n & (idx_t)1) == 0)   /* even n */
        val += (seq_t)8;
    else                       /* odd n */
        val -= (seq_t)8;

    return val;
}
