/*
 * A079297  Triangle read by rows: the k-th column is an arithmetic
 *          progression with difference 2k-1 and the top entry is the
 *          hexagonal number k*(2*k-1) (A000384).
 * https://oeis.org/A079297
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, tabl, easy
 * Formula:   a(n) = r*(2c-1), where * r = ceil((sqrt(1+8n)-1)/2), * c = n -
 *            r*(r-1)/2. * The sequence is the triangle read by rows: * T(r,k)
 *            = r*(2k-1) with 1 ≤ k ≤ r
 * Range:     u8 n <= 67, u16 n <= 67, u32 n <= 67, u64 n <= 67, u128 n <= 67,
 *            big any n
 * Helpers:   isqrt_u128
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include "../utils/isqrt_u128.h"

seq_t a(idx_t n) {
    if (n == 0) return (seq_t)0;
    if (n == 1) return (seq_t)1;

    /* Compute r: the row number containing index n */
    unsigned __int128 d = (unsigned __int128)8 * n + 1;
    idx_t s = isqrt_u128(d);
    idx_t r = (s - 1) / 2;
    unsigned __int128 sum = (unsigned __int128)r * (r + 1) / 2;
    if (sum < n) r++;                    /* ensure r*(r+1)/2 ≥ n */

    /* Column position within the row */
    idx_t prev_cum = (unsigned __int128)(r - 1) * r / 2;
    idx_t col = n - prev_cum;

    /* Triangle value: T(r, col) = r*(2*col-1) */
    seq_t answer = (seq_t)r * (((seq_t)2 * (seq_t)col) - 1);
    return answer;
}
