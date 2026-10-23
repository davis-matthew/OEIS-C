/*
 * A158387  a(n) = -1 if n is a square, 1 if n is not a square.
 * https://oeis.org/A158387
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  sign, easy
 * Formula:   a(n) = -1 if n is a perfect square, 1 otherwise
 * Range:     i8 n <= 9012, i16 n <= 9012, i32 n <= 9012, i64 n <= 9012,
 *            i128 n <= 9012, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>

/* Helper: integer square root for 64‑bit unsigned integers.
   Returns ⌊√n⌋.  No overflow occurs because the test mid ≤ n/mid
   is used instead of mid*mid. */
static inline idx_t int_sqrt_u64(idx_t n)
{
    /* For n = 0 the answer is 0. */
    if (n == 0)
        return 0;

    idx_t low = 0;
    idx_t high = (idx_t)1 << 32;   /* sqrt(2^64-1) < 2^32 */

    while (low <= high) {
        idx_t mid = low + ((high - low) >> 1);
        /* mid*mid may overflow, so compare via division. */
        if (mid > 0 && mid > n / mid) {
            high = mid - 1;
        } else {
            low = mid + 1;
        }
    }
    return high;   /* high is the floor of the square root */
}

seq_t a(idx_t n)
{
    /* a(1) is the first term of the sequence, n ≥ 1 as per offset. */
    idx_t r = int_sqrt_u64(n);

    /* n is a square iff r*r == n.
       Avoid multiplication overflow by checking n % r == 0 and r == n/r. */
    bool is_square = (r != 0 && n % r == 0 && r == n / r);

    return (seq_t)(is_square ? -1 : 1);
}
