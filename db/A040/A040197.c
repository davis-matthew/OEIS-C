/*
 * A040197  Continued fraction for sqrt(212).
 * https://oeis.org/A040197
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, cofr, easy
 * Formula:   not stated
 * Range:     u8 n <= 89, u16 n <= 89, u32 n <= 89, u64 n <= 89, u128 n <= 89,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* A040197 – continued fraction for √212.  
   a(0)=⌊√212⌋=14; for n≥1, a(n) is the n‑th partial quotient of the
   continued fraction expansion of √212.  The period after a(0) is 14. */

seq_t a(idx_t n)
{
    if (n == 0)
        return (seq_t)14;

    /* constants for √212 */
    const idx_t D  = 212;      /* the radicand */
    const idx_t a0 = 14;       /* floor(sqrt(D)) */

    /* initialize the continued‑fraction algorithm for quadratic surds */
    idx_t m = 0;
    idx_t d = 1;
    idx_t a = a0;

    /* compute the (n‑1)th step of the period (period=14) */
    idx_t pos = (n - 1) % 14;

    for (idx_t i = 0; i <= pos; ++i) {
        m = d * a - m;
        d = (D - m * m) / d;
        a = (a0 + m) / d;
    }

    return (seq_t)a;
}
