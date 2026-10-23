/*
 * A101080  Table of Hamming distances between binary vectors representing i
 *          and j, for i >= 0, j >= 0, read by antidiagonals.
 * https://oeis.org/A101080
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, nonn, tabl
 * Formula:   a(n) = popcount of i XOR j, where (i,j) is the pair reached by
 *            reading the infinite table of Hamming distances in antidiagonal
 *            order
 * Range:     u8 n <= 104, u16 n <= 104, u32 n <= 104, u64 n <= 104,
 *            u128 n <= 104, big any n
 * Helpers:   popcount_uint64, triangular_threshold
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>
#include "../utils/popcount_uint64.h"
#include "../utils/triangular_threshold.h"

seq_t a(idx_t n)
{
    unsigned long long t = triangular_threshold(n);
    unsigned long long s = t - 1;                       /* sum i + j on this diagonal */
    unsigned __int128 prefix128 = (unsigned __int128)s * (s + 1) / 2; /* count before this diagonal */
    idx_t r = n - (idx_t)prefix128;                    /* index within the diagonal (i) */

    idx_t i = r;
    idx_t j = s - r;
    idx_t x = i ^ j;                                   /* XOR of the two indices */

    return popcount_uint64((uint64_t)x);
}
