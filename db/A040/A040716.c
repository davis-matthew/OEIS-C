/*
 * A040716  Continued fraction for sqrt(744).
 * https://oeis.org/A040716
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

/* Continued fraction for sqrt(744): a(0)=27, and for n>0
   a(n) is the (n‑1)th partial quotient of the periodic
   continued fraction expansion of √744. */

#include <stddef.h>

seq_t a(idx_t n)
{
    if (n == 0u)
        return (seq_t)27;          /* a(0) = ⌊√744⌋ = 27 */

    /* Period of the continued fraction for √744.  It is
       computed only once and reused on subsequent calls. */
    static seq_t period[200];
    static size_t period_len = 0;

    if (period_len == 0) {
        const idx_t D   = 744;   /* The radicand */
        const idx_t a0  = 27;    /* ⌊√744⌋ */
        idx_t m = 0;            /* m_0 */
        idx_t d = 1;            /* d_0 */
        idx_t a = a0;           /* a_0 */

        do {
            /* Next triple (m,d,a) */
            m = d * a - m;
            d = (D - m * m) / d;
            a = (a0 + m) / d;

            period[period_len++] = (seq_t)a;
        } while (a != (a0 << 1));   /* stop when a == 2*a0 (54) */
    }

    /* a(n) for n>0: use the (n‑1)th element of the period */
    size_t idx = (size_t)(n - 1) % period_len;
    return period[idx];
}
