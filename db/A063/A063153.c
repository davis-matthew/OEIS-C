/*
 * A063153  Dimension of the space of weight 2n cusp forms for Gamma_0( 85 ).
 * https://oeis.org/A063153
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 18*n + (-1)^n - 11 for n > 1; a(1) = 7
 * Range:     u8 n <= 14, u16 n <= 50, u32 n <= 50, u64 n <= 50, u128 n <= 50,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)0;      /* out‑of‑range guard, not part of the sequence */
    if (n == 1) return (seq_t)7;

    /* compute 18*n - 11 + (-1)^n */
    seq_t res = ((seq_t)n * (seq_t)18) - (seq_t)11;
    if (n & (seq_t)1)  /* odd n → (-1)^n = -1 */
        res -= (seq_t)1;
    else               /* even n → (-1)^n =  1 */
        res += (seq_t)1;

    return res;
}
