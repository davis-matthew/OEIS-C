/*
 * A037453  Positive numbers whose base-5 representation contains no 3 or 4.
 * https://oeis.org/A037453
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy, base
 * Formula:   a(n) follows the recurrence a(3k) = 5*a(k) a(3k+1) = 5*a(k)+1
 *            a(3k+2) = 5*a(k)+2 a(0) = 0 Thus a(n) is obtained by
 *            interpreting the base‑3 digits of n as base‑5 digits
 * Range:     u8 n <= 57, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

static inline seq_t _a_rec(idx_t k)
{
    if (k == 0) return 0;
    return 5 * _a_rec(k / 3) + (seq_t)(k % 3);
}

seq_t a(idx_t n)
{
    return _a_rec(n);
}
