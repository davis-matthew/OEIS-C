/*
 * A054385  Beatty sequence for e/(e-1); complement of A022843.
 * https://oeis.org/A054385
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = ⌊n * e/(e-1)⌋ (Beatty sequence for e/(e-1))
 * Range:     u8 n <= 1, u16 none, u32 n <= 3, u64 n <= 10000, u128 n <= 10000,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* Rational approximation of e/(e-1) using a truncation of the
       series e = Σ_{k=0}^{12} 1/k!  (error < 10⁻¹⁰).  */
    const seq_t r_num = (seq_t)1302061345U;   /* numerator of r = e/(e-1)  */
    const seq_t r_den = (seq_t)823059745U;    /* denominator of r           */

    return (((seq_t)n) * r_num) / r_den;
}
