/*
 * A040321  Continued fraction for sqrt(340).
 * https://oeis.org/A040321
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, cofr, easy
 * Formula:   a(n) = the n‑th partial quotient of the simple continued
 *            fraction of sqrt(340)
 * Range:     u8 n <= 89, u16 n <= 89, u32 n <= 89, u64 n <= 89, u128 n <= 89,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdbool.h>

#define PERIOD_LENGTH 14   /* length of the repeating block */

/* period[0] = a(1), period[1] = a(2), …, period[PERIOD_LENGTH‑1] = a(14) */
static seq_t period[PERIOD_LENGTH];
static bool period_initialized = false;

static void init_period(void)
{
    /* constants for sqrt(340) */
    const uint64_t S = 340ULL;          /* D in sqrt(D) */
    const uint64_t sqrt_floor = 18ULL;  /* floor(sqrt(340)) = 18 */

    uint64_t m = 0;
    uint64_t d = 1;
    uint64_t a = sqrt_floor;           /* current a */

    uint64_t i = 0;
    for (;;)
    {
        uint64_t m_next = d * a - m;
        uint64_t d_next = (S - m_next * m_next) / d;
        uint64_t a_next = (sqrt_floor + m_next) / d_next; /* floor((sqrt_floor + m_next)/d_next) */

        period[i++] = (seq_t)a_next;    /* store the next partial quotient */

        if (d_next == 1)                /* period ends when d_next = 1 (a_next = 2*sqrt_floor) */
            break;

        m = m_next;
        d = d_next;
        a = a_next;
    }

    period_initialized = true;
}

seq_t a(idx_t n)
{
    if (n == 0)
        return (seq_t)18; /* a(0) = floor(sqrt(340)) */

    if (!period_initialized)
        init_period();

    /* periodic part starts at a(1) */
    idx_t idx = (n - 1) % PERIOD_LENGTH;
    return period[(size_t)idx];
}
