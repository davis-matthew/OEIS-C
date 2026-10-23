/*
 * A010196  Continued fraction for sqrt(137).
 * https://oeis.org/A010196
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, cofr, easy
 * Formula:   not stated
 * Range:     u8 n <= 77, u16 n <= 77, u32 n <= 77, u64 n <= 77, u128 n <= 77,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Continued fraction for sqrt(137).
   a(0) = 11; for n >= 1
   a(n) = period[(n-1) % period_len]
   where period = [1, 2, 2, 1, 1, 2, 2, 1, 22] and period_len = 9. */

#include <stddef.h>
#include <stdbool.h>

/* ------- Memoisation data ------- */
static seq_t period[20];
static size_t period_len = 0;   /* 0 means not yet computed */

/* ------- Compute the period of the continued fraction once ------- */
static void
init_period(void)
{
    if (period_len != 0)
        return;                 /* already initialised */

    const uint64_t D  = 137ULL;
    const seq_t   a0  = (seq_t)11;   /* floor(sqrt(137)) */

    uint64_t m = 0;
    uint64_t d = 1;
    seq_t   a = a0;

    size_t idx = 0;
    do
    {
        /* next triple (m, d, a) */
        uint64_t m_next = d * (uint64_t)a - m;
        uint64_t d_next = (D - m_next * m_next) / d;
        seq_t   a_next = (seq_t)((a0 + (seq_t)m_next) / (seq_t)d_next);

        period[idx++] = a_next;

        /* period ends when a_next == 2*a0 */
        if (a_next == (seq_t)(a0 * 2))
            break;

        m = m_next;
        d = d_next;
        a = a_next;
    } while (true);

    period_len = idx;           /* should be 9 for sqrt(137) */
}

/* ------- Main interface ------- */
seq_t
a(idx_t n)
{
    if (n == 0)
        return (seq_t)11;       /* a(0) */

    init_period();             /* lazy initialisation */

    size_t idx = (size_t)((n - 1) % period_len);
    return period[idx];
}
