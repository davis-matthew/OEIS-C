/*
 * A122064  Subtract 3, take the square root, subtract 3, take the square
 *          root, etc.
 * https://oeis.org/A122064
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, fini, full
 * Formula:   a(1)=147; for n>1: a(n)= (n even ? max(a(n-1)-3,0) : √(a(n-1)))
 * Range:     u8 n <= 2, u16 n <= 7, u32 n <= 7, u64 n <= 7, u128 n <= 7,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>

/* Integer square root for seq_t values */
static inline seq_t isqrt_seq(seq_t x)
{
    seq_t low = 0, high = x, mid, mid2;
    while (low <= high) {
        mid = (low + high) >> 1;
        mid2 = mid * mid;
        if (mid2 == x) return mid;
        if (mid2 < x) {
            low = mid + 1;
        } else {
            if (mid == 0) break;
            high = mid - 1;
        }
    }
    return low - 1;
}

seq_t a(idx_t n)
{
    if (n == 1) return (seq_t)147;

    seq_t curr = (seq_t)147;
    /* Iterate from 2 to n */
    for (idx_t i = 2; i <= n; ++i) {
        if ((i & 1) == 0) {      /* even step: subtract 3 */
            if (curr <= 3) {
                curr = 0;
            } else {
                curr -= 3;
            }
        } else {                  /* odd step: square root */
            curr = isqrt_seq(curr);
        }

        /* once curr becomes 0, it stays 0 for all subsequent steps */
        if (curr == 0 && i < n) {
            /* fast-forward remaining iterations */
            /* For remaining indices the result will remain 0 */
            return 0;
        }
    }
    return curr;
}
