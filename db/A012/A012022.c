/*
 * A012022  Expansion of e.g.f.: arctan(sin(arctan(x))) (odd powers only).
 * https://oeis.org/A012022
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign
 * Formula:   a(0) = 1, a(1) = -5 Recurrence: a(n+2) = - (
 *            4*(n+1)*(2n+1)*(2n+3)^2 * a(n) + (6n+11)*(2n+3) * a(n+1) ) where
 *            n >= 0. The function computes a(n) by iterating this 2‑term
 *            recurrence
 * Range:     i8 n <= 1, i16 n <= 3, i32 n <= 5, i64 n <= 9, i128 n <= 15,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)1;
    if (n == 1) return (seq_t)-5;

    seq_t a0 = (seq_t)1;          /* a(k)   */
    seq_t a1 = (seq_t)-5;         /* a(k+1) */

    for (idx_t k = 0; k <= n - 2; ++k)
    {
        /* (2k+3) is used several times – compute once */
        seq_t two_k_plus3 = (seq_t)(2 * k + 3);

        /* term1 = 4 * (k+1) * (2k+1) * (2k+3)^2 * a(k) */
        seq_t term1 = (seq_t)4;
        term1 *= (seq_t)(k + 1);
        term1 *= (seq_t)(2 * k + 1);
        term1 *= two_k_plus3;
        term1 *= two_k_plus3;
        term1 *= a0;

        /* term2 = (6k+11) * (2k+3) * a(k+1) */
        seq_t term2 = (seq_t)(6 * k + 11);
        term2 *= two_k_plus3;
        term2 *= a1;

        /* a(k+2) = - (term1 + term2) */
        seq_t next = -(term1 + term2);

        /* shift the pair for the next iteration */
        a0 = a1;
        a1 = next;
    }

    return a1;   /* a(n) */
}
