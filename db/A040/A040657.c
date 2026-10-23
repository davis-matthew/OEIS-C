/*
 * A040657  Continued fraction for sqrt(684).
 * https://oeis.org/A040657
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, cofr, easy
 * Formula:   not stated
 * Range:     u8 n <= 83, u16 n <= 83, u32 n <= 83, u64 n <= 83, u128 n <= 83,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Continued fraction terms for sqrt(684). */
#include <stddef.h>
#include <stdint.h>
#include <stdbool.h>

static seq_t period_buf[128];
static size_t period_len = 0;
static bool period_initialized = false;

static void compute_period(void)
{
    if (period_initialized)
        return;

    const int64_t N = 684;          /* non‑square */
    const int64_t a0 = 26;          /* floor(sqrt(684)) */

    int64_t m = 0;
    int64_t d = 1;
    int64_t a = a0;

    /* first periodic term */
    int64_t m_next = d * a - m;
    int64_t d_next = (N - m_next * m_next) / d;
    int64_t a_next = (a0 + m_next) / d_next;

    period_buf[0] = (seq_t)a_next;
    int64_t target_m = m_next;
    int64_t target_d = d_next;
    m = m_next;
    d = d_next;
    a = a_next;
    size_t idx = 1;

    while (1) {
        m_next = d * a - m;
        d_next = (N - m_next * m_next) / d;
        a_next = (a0 + m_next) / d_next;

        if (m_next == target_m && d_next == target_d)
            break;

        period_buf[idx++] = (seq_t)a_next;
        m = m_next;
        d = d_next;
        a = a_next;
    }

    period_len = idx;
    period_initialized = true;
}

seq_t a(idx_t n)
{
    if (!period_initialized)
        compute_period();

    if (n == 0)
        return (seq_t)26;

    size_t pos = (size_t)((n - 1) % (uint64_t)period_len);
    return period_buf[pos];
}
