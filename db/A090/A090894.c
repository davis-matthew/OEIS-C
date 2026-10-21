/*
 * A090894  Numbers in n-th downward diagonal of triangle T : 0; 1, 2; 3, 4,
 *          5; 6, 7, 8, 9; ...
 * https://oeis.org/A090894
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = floor( (ceil(n/(m+1))*2) ). In fact, a(n) is obtained by
 *            enumerating the infinite triangular array T(i,j) = i*(i+1)/2 + j
 *            (0 ≤ j ≤ i) along its downward diagonals (constant i+j). The
 *            sequence is the list of all T(i,j) sorted by i+j, breaking ties
 *            with increasing i. Implementation uses the...
 * Range:     u8 n <= 71, u16 n <= 71, u32 n <= 71, u64 n <= 71, u128 n <= 71,
 *            big any n
 * Helpers:   count_prefix
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/count_prefix.h"

seq_t a(idx_t n) {
    /* Find diagonal d such that T(d-1) ≤ n < T(d) */
    idx_t d = 0;
    while (count_prefix(d) <= n) d++;

    idx_t prev = (d == 0) ? 0 : count_prefix(d - 1);
    idx_t offset = n - prev;                  /* 0‑based position inside diagonal */
    idx_t i_start = (d + 1) >> 1;             /* ceil(d/2) */
    idx_t i = i_start + offset;
    idx_t j = d - i;                          /* because i + j = d */

    /* Compute T(i,j) = i*(i+1)/2 + j in seq_t arithmetic */
    seq_t i_seq = (seq_t)i;
    seq_t val = i_seq * (i_seq + 1) / 2 + (seq_t)j;
    return val;
}
