/*
 * A079643  a(n) = floor(n/floor(sqrt(n))).
 * https://oeis.org/A079643
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = floor(n / floor(sqrt(n)))
 * Range:     u8 n <= 1050, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   uint64_floor_sqrt
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include "../utils/uint64_floor_sqrt.h"

seq_t a(idx_t n) {
    /* offset starts at 1; for safety return 0 if called with 0 */
    if (n == 0)
        return 0;

    uint64_t s = uint64_floor_sqrt((uint64_t)n);   /* floor(sqrt(n)) */
    /* Division uses seq_t arithmetic; intermediate cast is fine. */
    return (seq_t)((seq_t)n / (seq_t)s);
}
