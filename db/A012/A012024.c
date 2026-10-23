/*
 * A012024  E.g.f. sinh(sin(arctan(x))) (odd powers only).
 * https://oeis.org/A012024
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  sign
 * Formula:   a(1)=1, a(2)=-2, a(3)=16. For n≥4 (1‑based) let k=n-1 be the
 *            0‑based index. a(k) = -2*(6*k*k-6*k+1)*a(k-1)
 *            -12*(k-1)^2*(2*k-3)*(2*k-1)*a(k-2)
 *            -4*(k-2)*(k-1)*(2*k-5)*(2*k-3)^2*(2*k-1)*a(k-3)
 * Range:     i8 n <= 4, i16 n <= 5, i32 n <= 7, i64 n <= 10, i128 n <= 14,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n < 1) return 0;
    if (n == 1) return (seq_t)1;
    if (n == 2) return (seq_t)-2;
    if (n == 3) return (seq_t)16;

    /* 0‑based indices: a0=1, a1=-2, a2=16 */
    seq_t a_k_3 = (seq_t)1;   /* a(k-3), initially a0 for k=3 */
    seq_t a_k_2 = (seq_t)-2;  /* a(k-2), initially a1 for k=3 */
    seq_t a_k_1 = (seq_t)16;  /* a(k-1), initially a2 for k=3 */
    seq_t a_k;                /* a(k) */

    for (idx_t k = 3; k < n; ++k) {
        seq_t kk = (seq_t)k;
        /* coefficient for a(k-1) */
        seq_t coef1 = (seq_t)-2 *
                      ( ((seq_t)6 * kk * kk) - ((seq_t)6 * kk) + (seq_t)1 );
        seq_t term1 = coef1 * a_k_1;

        /* coefficient for a(k-2) */
        seq_t coef2 = (seq_t)-12 *
                      ((kk - (seq_t)1) * (kk - (seq_t)1) *
                       ((seq_t)2 * kk - (seq_t)3) *
                       ((seq_t)2 * kk - (seq_t)1));
        seq_t term2 = coef2 * a_k_2;

        /* coefficient for a(k-3) */
        seq_t coef3 = (seq_t)-4 *
                      ((kk - (seq_t)2) *
                       (kk - (seq_t)1) *
                       ((seq_t)2 * kk - (seq_t)5) *
                       (((seq_t)2 * kk - (seq_t)3) *
                        ((seq_t)2 * kk - (seq_t)3)) *
                       ((seq_t)2 * kk - (seq_t)1));
        seq_t term3 = coef3 * a_k_3;

        a_k = term1 + term2 + term3;

        /* shift for next iteration */
        a_k_3 = a_k_2;
        a_k_2 = a_k_1;
        a_k_1 = a_k;
    }
    return a_k_1; /* a(n) */
}
