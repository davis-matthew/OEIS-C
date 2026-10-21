/*
 * A087279  Nonnegative numbers whose distance to the nearest positive square
 *          equals exactly 1.
 * https://oeis.org/A087279
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = k^2 + 1 if n = 2k, a(n) = k^2 - 1 if n = 2k+1, where k =
 *            floor(n/2). This follows from A087279: all numbers equal to a
 *            square ±1
 * Range:     u8 n <= 31, u16 n <= 511, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

seq_t a(idx_t n) {
    /* Index n starts at 1 (offset 1). */
    if (n < 1) return 0;                /* Undefined behavior for n=0, but return 0. */
    if ((n & 1ULL) == 0) {              /* Even index: a(2k) = k^2 + 1 */
        idx_t k = n >> 1;               /* k = n/2 */
        return (seq_t)k * k + 1;
    } else {                            /* Odd index: a(2k+1) = k^2 - 1, with k = (n+1)/2 */
        idx_t k = (n + 1) >> 1;         /* k = (n+1)/2 */
        return (seq_t)k * k - 1;
    }
}
