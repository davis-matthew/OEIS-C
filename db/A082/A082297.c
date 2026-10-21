/*
 * A082297  Main diagonal of array A083861.
 * https://oeis.org/A082297
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = (((5 + sqrt(4*n + 1))/2)^n - ((5 - sqrt(4*n +
 *            1))/2)^n)/sqrt(4*n + 1)
 * Range:     u8 n <= 4, u16 n <= 7, u32 n <= 13, u64 n <= 23, u128 n <= 23,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

seq_t a(idx_t n)
{
    /* handle the offset */
    if (n == 0)
        return 0;
    if (n == 1)
        return 1;

    /* recurrence derivation: a(k) = 5*a(k-1) + (n-6)*a(k-2)   (k ≥ 2) */
    seq_t a_prev2 = 0;          /* a(0) */
    seq_t a_prev1 = 1;          /* a(1) */

    /* coefficient (n-6) may be negative when n < 6 */
    int64_t coeff = (int64_t)n - 6;

    for (idx_t k = 2; k <= n; ++k) {
        seq_t a_curr;
        if (coeff >= 0) {
            a_curr = 5 * a_prev1 + (seq_t)coeff * a_prev2;
        } else {
            seq_t sub = (seq_t)(-coeff) * a_prev2;
            a_curr = 5 * a_prev1 - sub;
        }
        a_prev2 = a_prev1;
        a_prev1 = a_curr;
    }

    return a_prev1;
}
