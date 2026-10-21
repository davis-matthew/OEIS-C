/*
 * A239091  Prefix overlap of dictionary consisting of binary expansions of 0
 *          through n.
 * https://oeis.org/A239091
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   a(n) = Σ_{k=0}^{n-1} prefix_overlap(binary(k),binary(k+1))
 * Range:     u8 n <= 73, u16 n <= 6699, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Prefix overlap = number of common leading bits of two binary strings */

#include <stdint.h>
#include <stdlib.h>

/* Compute bit length of uint64_t, treating 0 as length 1 */
static inline int bitlen_uint64(uint64_t v)
{
    return v == 0 ? 1 : 64 - __builtin_clzll(v);
}

/* Number of common leading bits of a and b */
static inline int prefix_overlap_uint64(uint64_t a, uint64_t b)
{
    int len_a = bitlen_uint64(a);
    int len_b = bitlen_uint64(b);
    int min_len = len_a < len_b ? len_a : len_b;

    /* Align MSBs: shift longer number right so that its MSB aligns with the other's MSB */
    if (len_a > len_b) {
        a >>= (len_a - len_b);
    } else if (len_b > len_a) {
        b >>= (len_b - len_a);
    }

    uint64_t diff = a ^ b;
    if (diff == 0) return min_len;
    int diff_len = bitlen_uint64(diff);
    /* diff_len is the position of the most significant differing bit + 1 */
    return min_len - diff_len;
}

seq_t a(idx_t n)
{
    if (n == 0) return 0;  /* offset is 1, but return 0 for safety */
    seq_t sum = 0;
    for (idx_t i = 0; i < n; ++i) {
        uint64_t x = (uint64_t)i;
        uint64_t y = x + 1;
        sum += (seq_t)prefix_overlap_uint64(x, y);
    }
    return sum;
}
