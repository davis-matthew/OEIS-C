/*
 * A040780  Continued fraction for sqrt(809).
 * https://oeis.org/A040780
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, cofr, easy
 * Formula:   not stated
 * Range:     u8 n <= 86, u16 n <= 86, u32 n <= 86, u64 n <= 86, u128 n <= 86,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Continued fraction terms of sqrt(809). */
#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>

seq_t a(idx_t n)
{
    static bool            initialized = false;
    static seq_t           period[200];
    static size_t          period_len = 0;
    static seq_t           a0 = 0;          /* floor(sqrt(809)) */

    if (!initialized)
    {
        const idx_t        a0_int = 28;         /* floor(sqrt(809)) */
        a0 = (seq_t)a0_int;

        const unsigned long long D  = 809ULL;
        unsigned long long m   = 0;
        unsigned long long d   = 1;
        unsigned long long a   = a0_int;

        size_t idx = 0;
        while (true)
        {
            unsigned long long m_next = d * a - m;
            unsigned long long d_next = (D - m_next * m_next) / d;
            unsigned long long a_next = (a0_int + m_next) / d_next;

            period[idx++] = (seq_t)a_next;
            if (a_next == 2ULL * a0_int)
                break;

            m = m_next;
            d = d_next;
            a = a_next;
        }
        period_len = idx;
        initialized = true;
    }

    if (n == 0)
        return a0;

    size_t pos = (size_t)((n - 1) % period_len);
    return period[pos];
}
