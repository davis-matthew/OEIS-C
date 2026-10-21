/*
 * A081047  Difference of Stirling numbers of the first kind.
 * https://oeis.org/A081047
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, sign
 * Formula:   a(n) = n! − n!·Hₙ, where Hₙ = 1 + 1/2 + … + 1/n (harmonic
 *            number)
 * Range:     i8 n <= 4, i16 n <= 7, i32 n <= 12, i64 n <= 20, i128 n <= 33,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n == 0)   /* 0! – 0!*H₀ = 1 – 0 = 1 */
        return (seq_t)1;

    seq_t fact = (seq_t)1;   /* current factorial n!   */
    seq_t h    = (seq_t)0;   /* current n!·Hₙ  (integer) */

    for (idx_t i = 1; i <= n; ++i)
    {
        seq_t prev_fact = fact;          /* (i‑1)!  */
        fact = fact * (seq_t)i;          /* i! */

        if (i == 1)
            h = (seq_t)1;                /* 1!*H₁ = 1 */
        else
            h = (seq_t)i * h + prev_fact;/* i·h + (i‑1)!  */
    }

    return fact - h;   /* a(n) = n! − n!·Hₙ */
}
