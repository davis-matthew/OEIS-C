/*
 * A127616  a(n) = denominator of the continued fraction which has the
 *          positive integers which are <= n and are coprime to n as its
 *          terms. The terms are written in order from n-1 for the integer
 *          part, to 1 for the final term of the continued fraction.
 * https://oeis.org/A127616
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  frac, nonn
 * Formula:   not stated
 * Range:     u8 n <= 8, u16 n <= 10, u32 n <= 16, u64 n <= 22, u128 n <= 26,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* An implementation of OEIS sequence A127616
   a(n) = denominator of the continued fraction whose terms
   are the positive integers <= n that are coprime to n,
   listed from n-1 down to 1.
   The denominator is computed by the standard recurrence
   for continued‑fraction convergents. */
#include <stdint.h>

static inline idx_t gcd_uint64(idx_t a, idx_t b) {
    while (b) {
        idx_t t = a % b;
        a = b;
        b = t;
    }
    return a;
}

seq_t a(idx_t n) {
    if (n == 1) /* special case: list contains only 1 */
        return (seq_t)1;

    /* first term of the CF is n-1 (always coprime to n) */
    seq_t p_prev2 = 1;            /* numerator of q_-1   */
    seq_t p_prev1 = (seq_t)(n - 1);/* numerator after a0 */
    seq_t q_prev2 = 0;            /* denominator of q_-1 */
    seq_t q_prev1 = 1;            /* denominator after a0 */

    for (idx_t k = n - 2; k > 0; --k) {
        if (gcd_uint64(k, n) != 1)
            continue;

        seq_t a_k = (seq_t)k;
        seq_t p_new = a_k * p_prev1 + p_prev2;
        seq_t q_new = a_k * q_prev1 + q_prev2;

        p_prev2 = p_prev1; p_prev1 = p_new;
        q_prev2 = q_prev1; q_prev1 = q_new;
    }
    return q_prev1;
}
