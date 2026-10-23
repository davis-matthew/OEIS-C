/*
 * A028684  Pseudo Galois numbers for d=22.
 * https://oeis.org/A028684
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 22^n * ∏_{k=0}^{n-1} (22^n – 22^k)
 * Range:     u8 n <= 0, u16 n <= 1, u32 n <= 2, u64 n <= 3, u128 n <= 4,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    const seq_t d = (seq_t)22;

    if (n == 0)
        return (seq_t)1;

    /* d_pow_n = 22^n */
    seq_t d_pow_n = (seq_t)1;
    for (idx_t i = 0; i < n; ++i)
        d_pow_n *= d;

    /* product = ∏_{k=0}^{n-1} (d_pow_n – d^k) */
    seq_t product = (seq_t)1;
    seq_t d_pow_k = (seq_t)1;  /* 22^k, starting at k=0 */

    for (idx_t k = 0; k < n; ++k)
    {
        product *= (d_pow_n - d_pow_k);
        d_pow_k *= d;  /* advance to 22^(k+1) for next iteration */
    }

    return d_pow_n * product;
}
