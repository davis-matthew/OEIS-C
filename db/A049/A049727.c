/*
 * A049727  Array T read by diagonals; T(i,j)=number of lattice points in
 *          triangle (possibly degenerate) with vertices (0,0),(i,0),(i,j).
 * https://oeis.org/A049727
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, tabl
 * Formula:   a(n) = ((i*j + i + j + gcd(i,j)) / 2) + 1, where (i,j) are
 *            obtained from n by writing it as the offset within the
 *            antidiagonal of the array T: find k with k(k+1)/2 ≤ n <
 *            (k+1)(k+2)/2, set i = n - k(k+1)/2, j = k - i
 * Range:     u8 n <= 71, u16 n <= 71, u32 n <= 71, u64 n <= 71, u128 n <= 71,
 *            big any n
 * Helpers:   gcd, triangular128
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>
#include "../utils/gcd.h"
#include "../utils/triangular128.h"

seq_t a(idx_t n)
{
    /* Find diagonal index k: maximal k with k(k+1)/2 ≤ n */
    idx_t low = 0, high = n;
    while (low <= high) {
        idx_t mid = low + (high - low) / 2;
        __uint128_t tri = triangular128(mid);
        if (tri <= n)
            low = mid + 1;
        else
            high = (mid == 0) ? 0 : mid - 1;
    }
    idx_t k = high;  /* k(k+1)/2 ≤ n < (k+1)(k+2)/2 */

    /* Position within diagonal */
    idx_t offset = n - (idx_t)triangular128(k);
    idx_t i = offset;
    idx_t j = k - i;

    /* Compute T(i,j) */
    seq_t si = (seq_t)i;
    seq_t sj = (seq_t)j;
    seq_t g   = gcd(si, sj);
    seq_t prod = si * sj;
    seq_t sum  = prod + si + sj + g;
    seq_t res  = sum / 2 + 1;
    return res;
}
