/*
 * A155672  a(n) = 11^n-9^n+1^n.
 * https://oeis.org/A155672
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 11^n - 9^n + 1 Recurrence: a(0) = 1, a(1) = 3 a(n) =
 *            20*a(n-1) - 99*a(n-2) + 80 (n >= 2)
 * Range:     u8 n <= 2, u16 n <= 4, u32 n <= 9, u64 n <= 17, u128 n <= 17,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdint.h>

seq_t a(idx_t n)
{
    if (n == 0) {
        return (seq_t)1;
    }
    if (n == 1) {
        return (seq_t)3;
    }

    seq_t prev2 = (seq_t)1;   /* a(0) */
    seq_t prev1 = (seq_t)3;   /* a(1) */
    seq_t cur    = (seq_t)0;

    for (idx_t i = 2; i <= n; ++i) {
        cur = (seq_t)20 * prev1
            - (seq_t)99 * prev2
            + (seq_t)80;
        prev2 = prev1;
        prev1 = cur;
    }

    return cur;
}
