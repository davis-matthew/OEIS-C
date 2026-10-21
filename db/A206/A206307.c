/*
 * A206307  a(n) = ((2*n+2)*(2*n+3) - 1)*a(n-1) + 2*n*(2*n+1)*a(n-2), a(0)=0,
 *          a(1)=6.
 * https://oeis.org/A206307
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = ((2*n+2)*(2*n+3)-1)*a(n-1) + 2*n*(2*n+1)*a(n-2), a(0) =
 *            0, a(1) = 6
 * Range:     u8 n <= 2, u16 n <= 3, u32 n <= 5, u64 n <= 9, u128 n <= 15,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n == 0)
        return (seq_t)0;
    if (n == 1)
        return (seq_t)6;

    seq_t prev2 = (seq_t)0;   /* a(0) */
    seq_t prev1 = (seq_t)6;   /* a(1) */
    seq_t cur;

    for (idx_t k = 2; k <= n; ++k)
    {
        seq_t two_k = (seq_t)k * ((seq_t)2);
        seq_t coeff1 = ((two_k + (seq_t)2) * (two_k + (seq_t)3)) - (seq_t)1;
        seq_t coeff2 = two_k * (two_k + (seq_t)1);

        cur = coeff1 * prev1 + coeff2 * prev2;

        prev2 = prev1;
        prev1 = cur;
    }

    return prev1;
}
