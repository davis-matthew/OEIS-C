/*
 * A042776  Numerators of continued fraction convergents to sqrt(919).
 * https://oeis.org/A042776
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, cofr, frac, easy
 * Formula:   a(n) = numerator of convergents to sqrt(919)
 * Range:     u8 n <= 1, u16 n <= 10, u32 n <= 20, u64 n <= 38, u128 n <= 82,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

seq_t a(idx_t n)
{
    /*  floor(sqrt(919)) = 30 */
    const int a0 = 30;          /* a_0 */
    unsigned long long m = 0;   /* m_0 */
    unsigned long long d = 1;   /* d_0 */
    int a_k = a0;               /* a_k for current step */

    /* Convergent recurrence: h_-2 = 0, h_-1 = 1 */
    seq_t prev2 = 0;            /* h_{k-2} */
    seq_t prev1 = 1;            /* h_{k-1} */

    for (idx_t k = 0; k <= n; ++k) {
        /* Compute h_k */
        seq_t h = (seq_t)a_k * prev1 + prev2;

        if (k == n)
            return h;

        /* Prepare for next step */
        prev2 = prev1;
        prev1 = h;

        /* Next continued‑fraction term */
        m = d * a_k - m;
        d = (919ULL - m * m) / d;
        a_k = (a0 + (int)m) / (int)d;
    }
    /* This point should never be reached */
    return 0;
}
