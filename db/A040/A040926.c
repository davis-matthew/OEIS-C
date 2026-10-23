/*
 * A040926  Continued fraction for sqrt(957).
 * https://oeis.org/A040926
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, cofr, easy
 * Formula:   a(n) = the n-th partial quotient of the continued fraction of
 *            sqrt(957) (with a(0) = ⌊√957⌋)
 * Range:     u8 n <= 74, u16 n <= 74, u32 n <= 74, u64 n <= 74, u128 n <= 74,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdbool.h>

/* Return the n-th term of the continued fraction expansion of √957 */
seq_t a(idx_t n) {
    /* Static state to compute the period only once */
    static bool init = false;
    static seq_t a0;           /* integer part ⌊√957⌋ = 30 */
    static seq_t period[512];  /* periodic block, enough for any non‑square */
    static size_t period_len = 0;

    if (!init) {
        /* Compute a0 = ⌊√957⌋ using integer arithmetic (no float) */
        const int D = 957;
        int r = 0;
        for (; (r + 1) * (r + 1) <= D; ++r);
        a0 = (seq_t)r;               /* r == 30 */

        /* Generate the periodic part of the continued fraction */
        int m = 0;
        int d = 1;
        int a_cur = r;               /* a0 */
        const int a_double = r * 2;   /* 2 * a0, stop condition */

        size_t idx = 0;
        while (1) {
            /* Next values of the continued fraction algorithm */
            int m_next = d * a_cur - m;
            int d_next = (D - m_next * m_next) / d;
            int a_next = (r + m_next) / d_next;

            period[idx++] = (seq_t)a_next;   /* store as seq_t */

            if (a_next == a_double)
                break;                       /* period completed */

            m = m_next;
            d = d_next;
            a_cur = a_next;
        }

        period_len = idx;
        init = true;
    }

    if (n == 0)
        return a0;           /* integer part */

    /* Periodic part: indices start at 1 after a0 */
    size_t pos = (size_t)((n - 1) % (idx_t)period_len);
    return period[pos];
}
