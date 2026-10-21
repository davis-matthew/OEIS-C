/*
 * A216280  Number of nonnegative solutions to the equation x^4 + y^4 = n.
 * https://oeis.org/A216280
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 9928, u16 n <= 9928, u32 n <= 9928, u64 n <= 9928,
 *            u128 n <= 9928, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computes the number of unordered non‑negative integer solutions to
   x^4 + y^4 = n (i.e. the number of pairs (x,y) with x^4 + y^4 = n and
   y ≥ x).  Implemented by a simple brute‑force search. */

#include <stdint.h>

seq_t a(idx_t n)
{
    seq_t count = 0;
    for (idx_t x = 0; ; ++x) {
        /* x^4 */
        idx_t x4 = x * x * x * x;
        if (x4 > n) break;

        idx_t m = n - x4;          /* remaining value to be expressed as y^4 */

        /* find y such that y^4 == m (if any) */
        for (idx_t y = 0; ; ++y) {
            idx_t y4 = y * y * y * y;
            if (y4 == m) {        /* exact match found */
                if (y >= x)       /* ensure each unordered pair counted once */
                    ++count;
                break;
            }
            if (y4 > m) break;   /* no possible y for this x */
        }
    }
    return count;
}
