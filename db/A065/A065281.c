/*
 * A065281  Infinite binary tree inspired permutation of N: 1 -> 1, 11ab..yz
 *          -> 11ab..yz1, 10ab..y1 -> 10ab..y, 10ab..y0 -> 11ab..y0.
 * https://oeis.org/A065281
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) is the permutation defined on binary representations: 1 ->
 *            1, for n >= 2, let len = number of bits in n, let second = (n >>
 *            (len-2)) & 1; if second == 1 : a(n) = 2*n + 1; else if n is odd
 *            : a(n) = n >> 1; else : a(n) = n | (1ULL << (len-2));
 * Range:     u8 n <= 67, u16 n <= 67, u32 n <= 67, u64 n <= 67, u128 n <= 67,
 *            big any n
 * Helpers:   bitlen_uint64
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include "../utils/bitlen_uint64.h"

seq_t a(idx_t n) {
    if (n == 1)
        return (seq_t)1;
    uint64_t len = bitlen_uint64((uint64_t)n);
    uint64_t second = (uint64_t)n >> (len - 2);
    second &= 1ULL;
    if (second) {
        /* prepend 1 to the two most significant bits: 11... -> 11...1 */
        return (seq_t)n * (seq_t)2 + (seq_t)1;
    } else {
        if (n & 1ULL) {
            /* odd with prefix 10: drop the least significant bit */
            return (seq_t)(n >> 1);
        } else {
            /* even with prefix 10: set the second most significant bit to 1 */
            return (seq_t)(n | (1ULL << (len - 2)));
        }
    }
}
