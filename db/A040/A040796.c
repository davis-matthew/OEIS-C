/*
 * A040796  Continued fraction for sqrt(825).
 * https://oeis.org/A040796
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, cofr, easy
 * Formula:   a(n) is the n-th term of the simple continued fraction expansion
 *            of sqrt(825). The expansion is 28;(1,2,1,1,1,1,4,1,1,1,1,2,1,56)
 *            repeating after the first term
 * Range:     u8 n <= 89, u16 n <= 89, u32 n <= 89, u64 n <= 89, u128 n <= 89,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* D = 825 (non‑square) */
    const idx_t D = 825;
    /* The integer part of sqrt(825) */
    const idx_t a0 = 28;  /* floor(sqrt(825)) */

    if (n == 0)
        return (seq_t)a0;

    /* Intermediate values for the standard continued‑fraction algorithm */
    idx_t m = 0;
    idx_t d = 1;
    idx_t a_term = a0;        /* a contains a0 at the start */

    seq_t result = 0;

    /* iterate until we reach the n‑th term */
    for (idx_t i = 1; i <= n; ++i) {
        /* Compute the next triplet (m,d,a) */
        m = d * a_term - m;
        /* d = (D - m^2)/d  -- division is exact */
        d = (D - m * m) / d;
        /* a = floor((a0 + m)/d) */
        a_term = (a0 + m) / d;

        if (i == n)
            result = (seq_t)a_term;
    }
    return result;
}
