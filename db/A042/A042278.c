/*
 * A042278  Numerators of continued fraction convergents to sqrt(665).
 * https://oeis.org/A042278
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, cofr, frac, easy
 * Formula:   not stated
 * Range:     u8 n <= 3, u16 n <= 9, u32 n <= 19, u64 n <= 41, u128 n <= 85,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A042278: numerators of the continued‑fraction convergents to √665.
   Recurrence:
      p(-2) = 0,  p(-1) = 1
      a0     = floor(√665) = 25
      a_k (k ≥ 1) = [1, 3, 1, 2, 2, 2, 1, 3, 1, 50]   (period length 10)
      p_k = a_k * p_{k-1} + p_{k-2}   (for k ≥ 0)
   Then a(n) = p_n.  This runs in O(n) time and O(1) space. */

#include <stddef.h>
#include <stdint.h>

static const seq_t PERIOD[] = {
    (seq_t)1, (seq_t)3, (seq_t)1, (seq_t)2, (seq_t)2,
    (seq_t)2, (seq_t)1, (seq_t)3, (seq_t)1, (seq_t)50
};
static const size_t PERIOD_LEN = sizeof PERIOD / sizeof PERIOD[0];

seq_t a(idx_t n)
{
    const seq_t a0 = (seq_t)25;           /* floor(sqrt(665)) */

    /* p(-2)=0, p(-1)=1; use shifted notation: p_prevprev = p_{k-2}, p_prev = p_{k-1} */
    seq_t p_prevprev = (seq_t)1;          /* p_{-1} */
    seq_t p_prev    = a0;                 /* p_0 */

    if (n == 0) return p_prev;           /* a(0) = 25 */

    seq_t a_k, p_cur;
    for (idx_t k = 1; k <= n; ++k) {
        /* a_k for k>=1 comes from PERIOD[ (k-1) % PERIOD_LEN ] */
        a_k = PERIOD[(size_t)((k - 1) % PERIOD_LEN)];
        p_cur = a_k * p_prev + p_prevprev;
        p_prevprev = p_prev;
        p_prev     = p_cur;
    }
    return p_prev;                        /* p_n = a(n) */
}
