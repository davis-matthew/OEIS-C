/*
 * A040630  Continued fraction for sqrt(656).
 * https://oeis.org/A040630
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, cofr, easy, less
 * Formula:   not stated
 * Range:     u8 none, u16 n <= 89, u32 n <= 89, u64 n <= 89, u128 n <= 89,
 *            big any n
 * Helpers:   isqrt_local
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  Continued fraction expansion of sqrt(656).
 *
 *  a(0) = ⌊√656⌋ = 25.
 *  For n ≥ 1 the terms follow the periodic continued fraction of √656
 *  (period length 4: 1,1,1,1 then 50,1,1,1,1,2,1,1,1,1,50 …).
 *
 *  The algorithm computes the period once and then returns the appropriate
 *  term in O(1) time for any n via modulo arithmetic.
 */

#include <stddef.h>
#include <stdint.h>
#include "../utils/isqrt_local.h"

/* Return the n‑th term of the continued fraction of sqrt(656). */
seq_t a(idx_t n)
{
    /* static state initialised on first call */
    static int  initialized = 0;
    static seq_t a0;                    /* a(0) */
    static seq_t cf[128];               /* periodic part, cf[0] = a(1) */
    static size_t cf_len = 0;           /* number of periodic terms */
    static size_t period_len = 0;

    if (!initialized) {
        const seq_t N = (seq_t)656;
        a0 = isqrt_local(N);
        /* Start the continued fraction algorithm */
        seq_t m = 0, d = 1, a_cur = a0;
        size_t idx = 0;
        while (1) {
            seq_t m_next = d * a_cur - m;
            seq_t d_next = (N - m_next * m_next) / d;
            seq_t a_next = (a0 + m_next) / d_next;

            cf[idx++] = a_next;
            if (d_next == 1)    /* end of period */
                break;

            m = m_next;
            d = d_next;
            a_cur = a_next;
        }
        cf_len = idx;           /* number of terms stored (period length) */
        period_len = cf_len;
        initialized = 1;
    }

    if (n == 0)
        return a0;

    /* (n-1) modulo period length gives index into cf array */
    idx_t rem = (n - 1) % (idx_t)period_len;
    return cf[rem];
}
