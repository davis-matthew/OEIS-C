/*
 * A048697  Generalized Pellian with second term equal to 10.
 * https://oeis.org/A048697
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = 2*a(n-1) + a(n-2); a(0)=1, a(1)=10
 * Range:     u8 n <= 4, u16 n <= 11, u32 n <= 23, u64 n <= 48, u128 n <= 99,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

seq_t a(idx_t n)
{
    /* Handle the base cases */
    if (n == 0) return (seq_t)1;
    if (n == 1) return (seq_t)10;

    /* Iteratively compute using the recurrence */
    seq_t prev2 = (seq_t)1;   /* a(n-2) */
    seq_t prev1 = (seq_t)10;  /* a(n-1) */
    seq_t cur;

    for (idx_t i = 2; i <= n; ++i) {
        cur = (prev1 << 1) + prev2;  /* 2*a(n-1) + a(n-2) */
        prev2 = prev1;
        prev1 = cur;
    }
    return prev1;
}
