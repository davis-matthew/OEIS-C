/*
 * A054329  One quarter of fourth unsigned column of Lanczos' triangle
 *          A053125.
 * https://oeis.org/A054329
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = 4^(n-1) * binomial(2*n+4, 3) (a(0)=1)
 * Range:     u8 n <= 1, u16 n <= 4, u32 n <= 11, u64 n <= 25, u128 n <= 56,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)1;          /* a(0) = 1 */

    /* binomial(2n+4,3) = (2n+4)(2n+3)(2n+2) / 6  */
    seq_t bin = (seq_t)(2*n + 2);
    bin *= (seq_t)(2*n + 3);
    bin *= (seq_t)(2*n + 4);
    bin /= 6;

    /* multiply by 4^(n-1) by repeated multiplication */
    seq_t pow4 = 1;
    for (idx_t i = 1; i < n; ++i)  /* i runs from 1 to n-1 */
        pow4 *= 4;

    return bin * pow4;
}
