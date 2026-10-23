/*
 * A042781  Denominators of continued fraction convergents to sqrt(921).
 * https://oeis.org/A042781
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, frac, easy
 * Formula:   not stated
 * Range:     u8 n <= 4, u16 n <= 12, u32 n <= 23, u64 n <= 42, u128 n <= 84,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence: Denominators of the continued‑fraction convergents to sqrt(921).
   Computed by the recurrence:
          q_{-1} = 0,  q_0 = 1,
          q_k   = a_k * q_{k-1} + q_{k-2}   (k ≥ 1)
   where a_k are the periodic partial quotients of sqrt(921)
   (excluding the integer part 30).  The period has length 34:
   2,1,6,1,11,3,1,2,2,3,1,1,1,1,1,8,20,8,1,1,1,1,1,3,2,2,1,3,11,1,6,1,2,60. */

#include <stdint.h>
#include <stddef.h>

/* Periodic partial quotients of sqrt(921) (excluding the integer part).
   Length = 34. */
static const uint32_t period_a[34] = {
    2, 1, 6, 1, 11, 3, 1, 2, 2, 3,
    1, 1, 1, 1, 1, 8, 20, 8, 1, 1,
    1, 1, 1, 3, 2, 2, 1, 3, 11, 1,
    6, 1, 2, 60
};

seq_t a(idx_t n)
{
    /* Base cases */
    seq_t prev2 = 0;          /* q_{-1}  */
    seq_t prev1 = 1;          /* q_{0}   */

    if (n == 0)
        return prev1;          /* q_0 */

    const idx_t period_len = 34u;   /* length of the period */

    for (idx_t k = 1; k <= n; ++k) {
        /* a_k corresponds to period_a[(k-1) % period_len] */
        uint32_t ak = period_a[(size_t)((k - 1) % period_len)];
        seq_t q = (seq_t)ak * prev1 + prev2;
        prev2 = prev1;
        prev1 = q;
    }
    return prev1;              /* q_n */
}
