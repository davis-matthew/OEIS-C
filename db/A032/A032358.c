/*
 * A032358  Number of iterations of phi(n) needed to reach 2.
 * https://oeis.org/A032358
 *
 * Offset:    2    (a(2) is the first term)
 * Keywords:  nice, nonn, easy
 * Formula:   a(n) = a(phi(n)) + 1, a(1) = -1
 * Range:     u8 n <= 1201, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   phi
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include "../utils/phi.h"

seq_t a(idx_t n) {
    if (n == 1) return (seq_t)-1;          /* base case for recurrence */

    seq_t count = 0;
    seq_t v = (seq_t)n;

    /* iterate phi until the value becomes 2 */
    while (v != (seq_t)2) {
        v = phi((idx_t)v);
        ++count;
    }

    return count;
}
