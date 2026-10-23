/*
 * A040219  Continued fraction for sqrt(235).
 * https://oeis.org/A040219
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, cofr, easy
 * Formula:   a(n) = continued fraction coefficient of sqrt(235)
 * Range:     u8 n <= 74, u16 n <= 74, u32 n <= 74, u64 n <= 74, u128 n <= 74,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*          a(0) = floor(sqrt(235)) = 15,   a(1) = 3, a(2) = 30, ... */

#include <stdint.h>

seq_t a(idx_t n)
{
    /* D = 235, a0 = floor(sqrt(235)) = 15. */
    const seq_t a0 = (seq_t)15;
    if (n == 0) {
        return a0;
    }

    /* Initialize variables for the continued‑fraction algorithm */
    uint64_t m = 0;            /* previous m */
    uint64_t d = 1;            /* previous d */
    uint64_t a_cur = 15;       /* current a_i (already a0) */
    idx_t i;

    for (i = 1; i <= n; ++i) {
        uint64_t m_next = a_cur * d - m;
        uint64_t d_next = (235ULL - m_next * m_next) / d;
        uint64_t a_next = (15ULL + m_next) / d_next;
        m = m_next;
        d = d_next;
        a_cur = a_next;
    }
    return (seq_t)a_cur;
}
