/*
 * A125652  Numbers m such that m^2=A125650(k) for some k (belonging A125651).
 * https://oeis.org/A125652
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 3*A041053(2n-2) for n>1, a(1) = 1
 * Range:     u8 n <= 4, u16 n <= 7, u32 n <= 13, u64 n <= 21, u128 n <= 21,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* A041053 gives the denominators of the continued‑fraction convergents to sqrt(32) */
/* q_0 = 1, q_1 = 1, and for i≥2: q_i = a_i*q_{i-1} + q_{i-2}  */
/* where a_0 = 5, a_{i>0} follows the period [1,1,1,10] (i.e. a_i = 10 when (i-1)%4==3). */

#include <stddef.h>

seq_t a(idx_t n)
{
    if (n <= 1) return (seq_t)1;

    /* target index in the q‑sequence (q_0=1, q_1=1, ...) */
    idx_t target = 2 * n - 3;

    /* trivial cases */
    if (target == 0) return (seq_t)3 * (seq_t)1;
    if (target == 1) return (seq_t)3 * (seq_t)1;

    seq_t q_prev2 = (seq_t)1; /* q_0 */
    seq_t q_prev1 = (seq_t)1; /* q_1 */

    for (idx_t i = 2; i <= target; ++i) {
        idx_t cycle = (i - 1) & 3;          /* (i-1) mod 4 */
        seq_t a_i   = (cycle == 3) ? (seq_t)10 : (seq_t)1;
        seq_t q_curr = a_i * q_prev1 + q_prev2;
        q_prev2 = q_prev1;
        q_prev1 = q_curr;
    }

    return (seq_t)3 * q_prev1;
}
