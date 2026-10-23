/*
 * A014923  a(1) = 1, a(n) = n*9^(n-1) + a(n-1).
 * https://oeis.org/A014923
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 2, u16 n <= 5, u32 n <= 9, u64 n <= 19, u128 n <= 39,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*
 * Formula: a(1) = 1
 *          a(n) = n*9^(n-1) + a(n-1)   (n ≥ 2)
 *
 *  Equivalent linear recurrence (valid for n ≥ 3):
 *          a(1) = 1
 *          a(2) = 19
 *          a(n) = 18*a(n-1) - 81*a(n-2) + 1
 */

#include <stddef.h>   /* for size_t if needed */

seq_t a(idx_t n)
{
    if (n == 1) return (seq_t)1;
    if (n == 2) return (seq_t)19;

    seq_t prev2 = (seq_t)1;   /* a(1) */
    seq_t prev1 = (seq_t)19;  /* a(2) */
    seq_t cur   = (seq_t)0;

    for (idx_t i = 3; i <= n; ++i) {
        cur = (seq_t)18 * prev1 - (seq_t)81 * prev2 + (seq_t)1;
        prev2 = prev1;
        prev1 = cur;
    }
    return cur;
}
