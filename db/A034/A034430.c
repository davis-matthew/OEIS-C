/*
 * A034430  Convolution of A001147 (double factorial numbers) with itself.
 * https://oeis.org/A034430
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = (3*n-1)*a(n-1) - (2*n-1)*(n-1)*a(n-2) with a(0)=1, a(1)=2
 * Range:     u8 n <= 4, u16 n <= 6, u32 n <= 10, u64 n <= 17, u128 n <= 28,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n == 0)
        return 1;
    if (n == 1)
        return 2;
    seq_t prev2 = 1; /* a(0) */
    seq_t prev1 = 2; /* a(1) */
    seq_t cur;
    for (idx_t k = 2; k <= n; ++k) {
        seq_t coeff1 = (seq_t)(3*k - 1);
        seq_t coeff2 = (seq_t)(2*k - 1) * (seq_t)(k - 1);
        cur = coeff1 * prev1 - coeff2 * prev2;
        prev2 = prev1;
        prev1 = cur;
    }
    return prev1;
}
