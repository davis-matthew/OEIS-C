/*
 * A061854  Nondiving binary sequences: numbers which in base 2 have at least
 *          the same number of 1's as 0's and reading the binary expansion
 *          from left (msb) to right (least significant bit), the number of
 *          0's never exceeds the number of 1's.
 * https://oeis.org/A061854
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base, easy
 * Formula:   not stated
 * Range:     u8 n <= 147, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   ok_uint64
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Algorithm: Brute force enumeration of integers whose binary representation
 * satisfies the Dyck‑prefix condition (never more zeros than ones in any
 * prefix and total ones ≥ total zeros).  Returns the nth such integer
 * (index 1 is the first term). */

#include <stdbool.h>
#include "../utils/ok_uint64.h"

seq_t a(idx_t n)
{
    /* n is 1‑based index; we iterate integers until n valid numbers are found. */
    idx_t count = 0;
    uint64_t num = 1;
    for ( ; ; ++num) {
        if (ok_uint64(num)) {
            ++count;
            if (count == n)
                return (seq_t)num;
        }
    }
}
