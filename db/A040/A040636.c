/*
 * A040636  Continued fraction for sqrt(662).
 * https://oeis.org/A040636
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, cofr, easy, less
 * Formula:   a(n) = the n-th partial quotient of the continued fraction of
 *            sqrt(662) (a(0)=25). The sequence is periodic after the first
 *            term with period 22
 * Range:     u8 n <= 89, u16 n <= 89, u32 n <= 89, u64 n <= 89, u128 n <= 89,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* Constant part of the continued fraction */
    const idx_t D   = 662;
    const idx_t a0  = 25;

    if (n == 0) return (seq_t)a0;

    idx_t m = 0;
    idx_t d = 1;
    idx_t a = a0;

    for (idx_t i = 1; i <= n; ++i) {
        /* Standard continued‑fraction iteration for sqrt(D) */
        m = d * a - m;                     /* m_k   */
        d = (D - m * m) / d;               /* d_k   */
        a = (a0 + m) / d;                  /* a_k   */
    }
    return (seq_t)a;
}
