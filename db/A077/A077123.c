/*
 * A077123  Let M_n be the n X n matrix M_(i,j) = i!-j! then the
 *          characteristic polynomial of M_n = x^n+a(n)*x^(n-2).
 * https://oeis.org/A077123
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = n * Σ_{k=1}^n (k!)² – (Σ_{k=1}^n k!)² The sequence is
 *            defined by the coefficient of x^(n‑2) in the characteristic
 *            polynomial of the matrix M_n with M_{i,j} = i! – j!. This closed
 *            form can be obtained from the fact that M_n has rank 2 and its
 *            two non‑zero eigenvalues satisfy λ₁ + λ₂ = 0
 * Range:     u8 n <= 3, u16 n <= 5, u32 n <= 7, u64 n <= 12, u128 n <= 14,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* Guard for n == 0 – not part of the sequence, return 0. */
    if (n == 0) return 0;

    seq_t fact = 1;           /* k!   */
    seq_t sum_fact = 0;       /* Σ k! */
    seq_t sum_sq = 0;         /* Σ (k!)² */

    for (idx_t i = 1; i <= n; ++i) {
        fact = fact * (seq_t)i;           /* k!   */
        sum_fact += fact;                 /* Σ k! */
        sum_sq += fact * fact;            /* Σ (k!)² */
    }

    seq_t term1 = (seq_t)n * sum_sq;      /* n * Σ (k!)² */
    seq_t term2 = sum_fact * sum_fact;    /* (Σ k!)² */
    return term1 - term2;                 /* a(n) */
}
