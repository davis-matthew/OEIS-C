/*
 * A074226  Numbers n such that Kronecker(3,n) = 1.
 * https://oeis.org/A074226
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = n-th positive integer k such that Kronecker(3,k) = 1
 * Range:     u8 n <= 88, u16 n <= 1666, u32 n <= 1666, u64 n <= 1666,
 *            u128 n <= 1666, big any n
 * Helpers:   kronecker3_uint64
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>
#include "../utils/kronecker3_uint64.h"

seq_t a(idx_t n)
{
    idx_t found = 0;
    uint64_t candidate = 1ULL;

    while (1) {
        if (kronecker3_uint64(candidate) == 1)
            ++found;
        if (found == n)
            return (seq_t)candidate;
        ++candidate;
    }
}
