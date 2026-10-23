/*
 * A010182  Continued fraction for sqrt(117).
 * https://oeis.org/A010182
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, cofr, easy
 * Formula:   not stated
 * Range:     u8 n <= 1000, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  Continued‐fraction coefficients of √117.
    a(0) = 10,
    for n ≥ 1  a(n) = the (n-1)-th element of the repeating block
    [1, 4, 2, 4, 1, 20]   (continued fraction of √117).
*/
#include <stdbool.h>

seq_t a(idx_t n)
{
    /* Handle the first term separately. */
    if (n == 0)
        return (seq_t)10;

    /* Periodic block found once and reused. */
    static bool initialized = false;
    static seq_t period[64];
    static int period_len = 0;

    if (!initialized) {
        const seq_t S   = (seq_t)117;   /* the number under the square root   */
        const seq_t a0  = (seq_t)10;    /* floor(sqrt(117))                    */

        seq_t m   = (seq_t)0;
        seq_t d   = (seq_t)1;
        seq_t a_v = a0;                 /* current a_k                          */

        int idx = 0;
        while (true) {
            /* Next terms of the (m,d,a) recurrence. */
            seq_t m_next = d * a_v - m;
            seq_t d_next = (S - m_next * m_next) / d;
            seq_t a_next = (a0 + m_next) / d_next;

            period[idx++] = a_next;     /* store coefficient a_{k+1}            */
            if (a_next == (a0 + a0))    /* period ends when a = 2*a0            */
                break;

            m = m_next;
            d = d_next;
            a_v = a_next;
        }
        period_len = idx;
        initialized = true;
    }

    /* n ≥ 1: return the appropriate element from the periodic block. */
    idx_t rel = (n - 1) % period_len;
    return period[rel];
}
