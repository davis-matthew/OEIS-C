/*
 * A081901  Second binomial transform of binomial(n+5, 5).
 * https://oeis.org/A081901
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 3^n*(n^5 + 65*n^4 + 1385*n^3 + 11575*n^2 + 35574*n +
 *            29160)/29160 This implementation uses the linear recurrence
 *            derived from the generating function: a(n) = 18*a(n-1)
 *            -135*a(n-2) +540*a(n-3) -1215*a(n-4) +1458*a(n-5) -729*a(n-6),
 *            with initial values a(0)=1, a(1)=8, a(2)=49, a(3)=262, a(4)...
 * Range:     u8 n <= 2, u16 n <= 6, u32 n <= 14, u64 n <= 32, u128 n <= 70,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

static const seq_t INIT[6] = {(seq_t)1, (seq_t)8, (seq_t)49, (seq_t)262, (seq_t)1286, (seq_t)5944};

seq_t a(idx_t n)
{
    if (n < 6)
        return INIT[n];

    /* Buffer holds the last six computed terms: buf[0] = a_{k-5}, ..., buf[5] = a_k */
    seq_t buf[6];
    for (int i = 0; i < 6; ++i)
        buf[i] = INIT[i];

    /* Compute terms up to a(n) */
    for (idx_t k = 6; k <= n; ++k) {
        /* Linear recurrence with alternating signs */
        seq_t cur = buf[5] * 18;
        cur -= buf[4] * 135;
        cur += buf[3] * 540;
        cur -= buf[2] * 1215;
        cur += buf[1] * 1458;
        cur -= buf[0] * 729;

        /* Shift buffer left to make room for the new value */
        buf[0] = buf[1];
        buf[1] = buf[2];
        buf[2] = buf[3];
        buf[3] = buf[4];
        buf[4] = buf[5];
        buf[5] = cur;
    }

    return buf[5];
}
