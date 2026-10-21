/*
 * A063102  Dimension of the space of weight 2n cusp forms for Gamma_0( 34 ).
 * https://oeis.org/A063102
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = ((-1)^n + 18*n - 13)/2 for n > 1, a(1) = 3. This gives
 *            the dimensions of the space of weight 2n cusp forms for Γ₀(34).
 *            The formula yields all listed terms and is simple to compute for
 *            any n ≥ 1
 * Range:     u8 n <= 14, u16 n <= 50, u32 n <= 50, u64 n <= 50, u128 n <= 50,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n == 1)           /* offset 1 */
        return (seq_t)3;

    /* For n > 1 the numerator is always positive:
       even n → 18*n - 12
       odd  n → 18*n - 14                      */
    seq_t prod = (seq_t)18 * n;
    seq_t sub  = (n & (idx_t)1) ? (seq_t)14 : (seq_t)12;
    return (prod - sub) / (seq_t)2;
}
