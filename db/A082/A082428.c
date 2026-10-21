/*
 * A082428  a(1) = 1; a(n) = 3 + n * Sum_{k=1..n-1} a(k).
 * https://oeis.org/A082428
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 3 + n * Sum_{k=1..n-1} a(k) (with a(1)=1)
 * Range:     u8 n <= 4, u16 n <= 7, u32 n <= 11, u64 n <= 19, u128 n <= 22,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>

seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)0;          /* not part of offset but defensive */
    if (n == 1) return (seq_t)1;          /* base case */

    seq_t sum = (seq_t)1;                 /* S(1) = a(1) */
    seq_t a_val = (seq_t)1;               /* placeholder for a(n) */

    for (idx_t i = 2; i <= n; ++i) {
        a_val = (seq_t)3 + (seq_t)i * sum;/* a(i) = 3 + i * S(i-1) */
        sum += a_val;                    /* S(i) = S(i-1) + a(i)   */
    }

    return a_val;                         /* a(n) after loop */
}
