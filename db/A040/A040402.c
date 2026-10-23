/*
 * A040402  Continued fraction for sqrt(423).
 * https://oeis.org/A040402
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, cofr, easy
 * Formula:   not stated
 * Range:     u8 n <= 0, u16 n <= 87, u32 n <= 87, u64 n <= 87, u128 n <= 87,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  A040402 – continued fraction of √423
 *  a(0) = 20
 *  a(1..8) = 1,1,3,4,3,1,1,40
 *  The block 1,1,3,4,3,1,1,40 repeats periodically. */
#include <stdbool.h>
#include <stddef.h>

seq_t a(idx_t n)
{
    /*   Constant part a0 = ⌊√423⌋ = 20   */
    static const seq_t a0 = (seq_t)20;
    static const seq_t D  = (seq_t)423;

    /*   Periodic part of the continued fraction – computed once   */
    static seq_t period[64];
    static size_t period_len = 0;

    if (n == (idx_t)0)
        return a0;                      /* a(0) */

    /*   Compute period lazily – only once  */
    if (period_len == 0) {
        uint64_t m  = 0;
        uint64_t d  = 1;
        uint64_t a_cur = (uint64_t)a0;

        while (1) {
            /*   m_k   = d_{k-1} * a_{k-1} - m_{k-1}   */
            m = d * a_cur - m;

            /*   d_k   = (D - m_k^2) / d_{k-1}          */
            d = (uint64_t)(D - (seq_t)m * (seq_t)m) / d;

            /*   a_k   = (a0 + m_k) / d_k              */
            a_cur = ((uint64_t)a0 + m) / d;

            /*   Store the coefficient   */
            if (period_len < sizeof(period)/sizeof(period[0]))
                period[period_len++] = (seq_t)a_cur;
            else
                break;   /* should never happen for √423 */

            /*   End of period when a_k = 2*a0 = 40   */
            if (a_cur == ((uint64_t)a0 << 1))
                break;
        }
    }

    /*   a(1) corresponds to period[0]   */
    size_t idx = (size_t)(n - (idx_t)1) % period_len;
    return period[idx];
}
