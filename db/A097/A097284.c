/*
 * A097284  Rectangular array T by antidiagonals: T(n,k) = rank of k-th n in
 *          A097283.
 * https://oeis.org/A097284
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, tabl
 * Formula:   T(n,k) = position of the k-th n in A097283, read by
 *            antidiagonals. A097283 lists the pairs (i,j), 0<i<j, by sum
 *            s=i+j and then by i, writing i then j for each pair. The k-th n
 *            is a j-component when k<n (s=n+k), and an i-component when k>=n
 *            (s=n+k+1)
 * Range:     u8 n <= 72, u16 n <= 72, u32 n <= 72, u64 n <= 72, u128 n <= 72,
 *            big any n
 * Validated: partial, pass, 72 terms through n = 72 (2026-11-07)
 * Helpers:   isqrt
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include "../utils/isqrt.h"

static seq_t A097284_terms_up_to_sum(seq_t s) {
    if (s < 3) return 0;
    seq_t p = s / 2;
    if ((s & 1) == 0) return 2 * p * (p - 1);
    return 2 * p * p;
}

seq_t a(idx_t n) {
    idx_t d = (isqrt(8 * n + 1) - 1) / 2;
    while (d * (d + 1) / 2 < n) d++;
    while (d > 1 && (d - 1) * d / 2 >= n) d--;
    idx_t r = n - d * (d - 1) / 2;   /* 1..d */
    seq_t row = (seq_t)r;
    seq_t col = (seq_t)d + 1 - (seq_t)r;
    seq_t s, off;
    if (col < row) {
        s = row + col;
        off = 2 * (col - 1) + 1;
    } else {
        s = row + col + 1;
        off = 2 * (row - 1);
    }
    return A097284_terms_up_to_sum(s - 1) + off + 1;
}
