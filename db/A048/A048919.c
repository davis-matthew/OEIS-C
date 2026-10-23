/*
 * A048919  Indices of 9-gonal numbers which are also heptagonal.
 * https://oeis.org/A048919
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(1)=1, a(2)=88; a(n)=142*a(n-1)-a(n-2)-50 for n>=3
 * Range:     u8 n <= 2, u16 n <= 3, u32 n <= 5, u64 n <= 10, u128 n <= 19,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdint.h>

seq_t a(idx_t n)
{
    if (n == 1) return (seq_t)1;
    if (n == 2) return (seq_t)88;
    seq_t prev2 = (seq_t)1;   /* a(n-2) */
    seq_t prev1 = (seq_t)88;  /* a(n-1) */
    seq_t curr;
    for (idx_t i = 3; i <= n; ++i) {
        curr = (seq_t)142 * prev1 - prev2 - (seq_t)50;
        prev2 = prev1;
        prev1 = curr;
    }
    return prev1; /* a(n) */
}
