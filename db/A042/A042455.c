/*
 * A042455  Denominators of continued fraction convergents to sqrt(755).
 * https://oeis.org/A042455
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, frac, easy
 * Formula:   Denominators of continued fraction convergents to sqrt(755)
 *            Recurrence: q_{-1}=0, q_0=1, q_k = a_k * q_{k-1} + q_{k-2} where
 *            a_k repeats with period 4: [2,10,2,54] (a_1=2, a_2=10, a_3=2,
 *            a_4=54). The function computes q_n for n >= 0
 * Range:     u8 n <= 3, u16 n <= 6, u32 n <= 11, u64 n <= 23, u128 n <= 45,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>

static const seq_t a_coeff[] = { 2, 10, 2, 54 }; /* a_1 .. a_4 */
static const size_t a_period = sizeof(a_coeff) / sizeof(a_coeff[0]);

seq_t a(idx_t n)
{
    if (n == 0) {
        return (seq_t)1; /* q_0 */
    }

    seq_t prevprev = (seq_t)0; /* q_{-1} */
    seq_t prev     = (seq_t)1; /* q_0 */
    seq_t cur;

    for (idx_t k = 1; k <= n; ++k) {
        size_t idx = (size_t)((k - 1) % a_period);
        seq_t ak   = a_coeff[idx];
        cur = ak * prev + prevprev;
        prevprev = prev;
        prev = cur;
    }

    return prev; /* q_n */
}
