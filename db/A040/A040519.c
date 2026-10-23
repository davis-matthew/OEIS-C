/*
 * A040519  Continued fraction for sqrt(543).
 * https://oeis.org/A040519
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, cofr, easy, less
 * Formula:   Continued fraction for sqrt(543). Sequence of partial quotients
 *            of the periodic continued fraction expansion of sqrt(543)
 * Range:     u8 none, u16 n <= 84, u32 n <= 84, u64 n <= 84, u128 n <= 84,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdbool.h>


seq_t a(idx_t n)
{
    static bool built = false;
    static seq_t cycle[64];
    static size_t period = 0;
    const seq_t D  = (seq_t)543;
    const seq_t a0 = (seq_t)23;

    if (!built) {
        seq_t m = (seq_t)0;
        seq_t d = (seq_t)1;
        seq_t a_prev = a0;
        size_t i = 0;

        while (1) {
            seq_t m_new = d * a_prev - m;
            seq_t d_new = (D - m_new * m_new) / d;
            seq_t a_new = (a0 + m_new) / d_new;

            cycle[i++] = a_new;
            if (a_new == a0 + a0) {  /* period ends when a_n = 2*a0 */
                period = i;
                break;
            }
            m = m_new;
            d = d_new;
            a_prev = a_new;
        }
        built = true;
    }

    if (n == 0) return a0;

    size_t idx = (size_t)((n - 1) % period); /* shift so a(1) maps to cycle[0] */
    return cycle[idx];
}
