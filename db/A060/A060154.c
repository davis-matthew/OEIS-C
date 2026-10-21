/*
 * A060154  Table T(n,k) by antidiagonals of n^k mod k [n,k >= 1].
 * https://oeis.org/A060154
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, tabl
 * Formula:   a(n) = n^k mod k where (n,k) is the n-th entry along the
 *            antidiagonals of the table T(n,k)
 * Range:     u8 n <= 9870, u16 n <= 9870, u32 n <= 9870, u64 n <= 9870,
 *            u128 n <= 9870, big any n
 * Helpers:   mod_pow
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>
#include "../utils/mod_pow.h"

seq_t a(idx_t n)
{
    /* Find the smallest diagonal d with
       d*(d+1)/2 >= n.  Use a binary search on d. */
    uint64_t low = 1, high = 1;
    while ((unsigned __int128)high * (high + 1) / 2 < n)
        high <<= 1;
    while (low < high) {
        uint64_t mid = low + (high - low) / 2;
        unsigned __int128 tri = (unsigned __int128)mid * (mid + 1) / 2;
        if (tri >= n)
            high = mid;
        else
            low = mid + 1;
    }

    uint64_t d = low;                            /* diagonal index, ≥1 */
    unsigned __int128 tri_prev = (unsigned __int128)(d - 1) * d / 2;
    uint64_t offset = (uint64_t)(n - tri_prev - 1); /* 0‑based offset on the diagonal */
    uint64_t i = offset + 1;                     /* n-coordinate */
    uint64_t j = d - offset;                     /* k-coordinate */

    return mod_pow((seq_t)i, j, (seq_t)j);
}
