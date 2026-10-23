/*
 * A040841  Continued fraction for sqrt(871).
 * https://oeis.org/A040841
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, cofr, easy
 * Formula:   not stated
 * Range:     u8 n <= 88, u16 n <= 88, u32 n <= 88, u64 n <= 88, u128 n <= 88,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* A040841: terms of the simple continued fraction expansion of sqrt(871).
   a(0) = floor(sqrt(871)) = 29.
   For n > 0, a(n) is the (n‑1)-th element of the repeating period. */

#include <stdbool.h>
#include <stdint.h>

/* integer square root of a 64‑bit unsigned integer, floor(sqrt(v)) */
static inline uint64_t int_sqrt_u64(uint64_t v)
{
    uint64_t lo = 0, hi = v, mid, best = 0;
    while (lo <= hi) {
        mid = (lo + hi) >> 1;
        uint64_t sq = mid * mid;
        if (sq <= v) {
            best = mid;
            lo = mid + 1;
        } else {
            hi = mid - 1;
        }
    }
    return best;
}

seq_t a(idx_t n)
{
    /* static state: repeated part of the continued fraction */
    static bool initialized = false;
    static seq_t period[64];
    static idx_t period_len = 0;
    static seq_t a0 = 0;

    if (!initialized) {
        const uint64_t N = 871ULL;

        /* a0 is the integer part (floor sqrt of N) */
        uint64_t a0_uint = int_sqrt_u64(N);
        a0 = (seq_t)a0_uint;           /* fits in any seq_t */

        uint64_t m = 0;
        uint64_t d = 1;
        uint64_t a_cur = a0_uint;

        period_len = 0;
        while (1) {
            uint64_t m_next = d * a_cur - m;
            uint64_t d_next = (N - m_next * m_next) / d;
            uint64_t a_next = (a0_uint + m_next) / d_next;

            period[period_len] = (seq_t)a_next;
            period_len++;

            if (a_next == 2ULL * a0_uint)
                break;   /* period ends when a_next = 2*a0 */

            m = m_next;
            d = d_next;
            a_cur = a_next;
        }
        initialized = true;
    }

    if (n == 0)
        return a0;

    /* index into the period: (n-1) modulo period length */
    idx_t idx = (n - 1) % period_len;
    return period[idx];
}
