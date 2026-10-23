/*
 * A140455  13-Fibonacci sequence.
 * https://oeis.org/A140455
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 13*a(n-1) + a(n-2); a(0)=0, a(1)=1
 * Range:     u8 n <= 3, u16 n <= 5, u32 n <= 9, u64 n <= 18, u128 n <= 35,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>

seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)0;
    if (n == 1) return (seq_t)1;

    seq_t prev2 = (seq_t)0;   /* a(0) */
    seq_t prev1 = (seq_t)1;   /* a(1) */
    seq_t curr;

    for (idx_t i = 2; i <= n; ++i) {
        curr = (seq_t)(13 * prev1 + prev2);
        prev2 = prev1;
        prev1 = curr;
    }
    return prev1;
}
