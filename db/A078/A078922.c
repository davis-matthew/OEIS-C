/*
 * A078922  a(n) = 11*a(n-1) - a(n-2) with a(1)=1, a(2) = 10.
 * https://oeis.org/A078922
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 11*a(n-1) - a(n-2), a(1)=1, a(2)=10
 * Range:     u8 n <= 3, u16 n <= 5, u32 n <= 10, u64 n <= 19, u128 n <= 38,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

seq_t a(idx_t n)
{
    if (n == 1) return (seq_t)1;
    if (n == 2) return (seq_t)10;

    seq_t prev = (seq_t)1;   /* a(1) */
    seq_t curr = (seq_t)10;  /* a(2) */
    for (idx_t i = 3; i <= n; ++i) {
        seq_t next = curr * (seq_t)11 - prev;
        prev = curr;
        curr = next;
    }
    return curr;
}
