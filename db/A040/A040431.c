/*
 * A040431  Continued fraction for sqrt(453).
 * https://oeis.org/A040431
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, cofr, easy, less
 * Formula:   not stated
 * Range:     u8 n <= 77, u16 n <= 77, u32 n <= 77, u64 n <= 77, u128 n <= 77,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence: continued fraction for sqrt(453).
   a(0) = floor(√453) = 21
   For n >= 1, a(n) = period[(n-1) mod period_len]
   where period = [3,1,1,10,14,10,1,1,3,42]  (length 10). */

#include <stddef.h>   /* for size_t */

static idx_t _int_sqrt_u64(idx_t n) {
    idx_t low = 0, high = n;
    while (low <= high) {
        idx_t mid = low + (high - low) / 2;
        if (mid * mid <= n) {
            low = mid + 1;
        } else {
            high = mid - 1;
        }
    }
    return high;     /* floor(sqrt(n)) */
}

seq_t a(idx_t n) {
    static int initialized = 0;
    static idx_t a0 = 0;
    static seq_t period[10];
    static size_t period_len = 0;

    if (!initialized) {
        const idx_t D = 453;
        a0 = _int_sqrt_u64(D);          /* 21 */

        idx_t m = 0;
        idx_t d = 1;
        idx_t a_cur = a0;

        /* Generate the period terms of the continued fraction */
        while (1) {
            idx_t m_next = d * a_cur - m;
            idx_t d_next = (D - m_next * m_next) / d;
            idx_t a_next = (a0 + m_next) / d_next;

            period[period_len++] = (seq_t)a_next;

            if (a_next == a0 * 2)   /* end of period, a_next = 2*a0 */
                break;

            m = m_next;
            d = d_next;
            a_cur = a_next;
        }

        initialized = 1;
    }

    if (n == 0)
        return (seq_t)a0;

    return period[(n - 1) % period_len];
}
