/*
 * A139706  Take n in binary. Rotate the binary digits to the right until a 1
 *          once again appears as the leftmost digit. Convert back into
 *          decimal for a(n).
 * https://oeis.org/A139706
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base, easy, changed
 * Formula:   not stated
 * Range:     u8 n <= 255, u16 n <= 8192, u32 n <= 8192, u64 n <= 8192,
 *            u128 n <= 8192, big any n
 * Helpers:   floor_log2_uint64
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Algorithm: rotate the binary representation of n to the right until the
 * most‑significant bit becomes 1 again, then convert back to decimal. */
#include <stdint.h>
#include "../utils/floor_log2_uint64.h"

seq_t a(idx_t n)
{
    /* n ≥ 1 */
    /* bit length of n */
    unsigned len = floor_log2_uint64(n) + 1;

    /* count trailing zeros to locate the first 1 from the LSB side */
    unsigned tz = 0;
    uint64_t tmp = n;
    while ((tmp & 1ULL) == 0) {
        ++tz;
        tmp >>= 1;
    }
    unsigned k = tz + 1;          /* rotations needed */

    /* low mask of the k lowest bits of n */
    uint64_t low;
    if (k == 64) {
        low = n;                   /* all bits are low */
    } else {
        low = n & ((1ULL << k) - 1);
    }

    /* rotate right by k positions */
    uint64_t part1 = (k == 64) ? 0 : (n >> k);
    uint64_t part2 = low << (len - k);

    uint64_t res = part1 | part2;
    return (seq_t)res;
}
