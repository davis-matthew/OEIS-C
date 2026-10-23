/*
 * A040251  Continued fraction for sqrt(268).
 * https://oeis.org/A040251
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, cofr, easy
 * Formula:   a(n) = the nth partial quotient of the continued fraction for
 *            sqrt(268). a(0) = floor(sqrt(268)) = 16; for n > 0 use the
 *            standard recurrence for quadratic irrationals: m_{k+1} = d_k *
 *            a_k - m_k, d_{k+1} = (268 - m_{k+1}^2) / d_k, a_{k+1} =
 *            floor((a0 + m_{k+1}) / d_{k+1})
 * Range:     u8 n <= 1, u16 n <= 86, u32 n <= 86, u64 n <= 86, u128 n <= 86,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computes the nth term of OEIS A040251: continued fraction for sqrt(268). */
seq_t a(idx_t n)
{
    if (n == 0)
        return (seq_t)16;          /* a0 = floor(sqrt(268)) */

    const seq_t a0 = 16;           /* constant part of the continued fraction */
    seq_t m = 0;
    seq_t d = 1;
    seq_t a = a0;

    /* iterate n times to reach a(n) */
    for (idx_t i = 0; i < n; ++i) {
        m = d * a - m;
        d = ((seq_t)268 - m * m) / d;
        a = (a0 + m) / d;
    }

    return a;
}
