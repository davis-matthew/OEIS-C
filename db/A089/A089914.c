/*
 * A089914  a(n) = 3^n *n! *L_{n}^{-1/3}(-1), where L_n^{alpha}(x) are
 *          generalized Laguerre polynomials.
 * https://oeis.org/A089914
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(0)=1, a(1)=5 a(n) = (6*n-1)*a(n-1) - (n-1)*(9*n-12)*a(n-2) for
 *            n ≥ 2
 * Range:     u8 n <= 2, u16 n <= 4, u32 n <= 7, u64 n <= 13, u128 n <= 24,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computes the OEIS sequence A089914. */
seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)1;
    if (n == 1) return (seq_t)5;

    seq_t a0 = (seq_t)1; /* a(0) */
    seq_t a1 = (seq_t)5; /* a(1) */
    seq_t ai = a1;

    for (idx_t i = 2; i <= n; ++i) {
        seq_t coeff1 = (seq_t)(6 * i - 1);
        seq_t coeff2 = (seq_t)(i - 1);
        seq_t coeff3 = (seq_t)(9 * i - 12);
        ai = coeff1 * a1 - coeff2 * coeff3 * a0;
        a0 = a1;
        a1 = ai;
    }
    return ai;
}
