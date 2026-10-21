/*
 * A077264  Table of arithmetic sequences, by antidiagonals.
 * https://oeis.org/A077264
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, tabl
 * Formula:   a(n) = value in the table of arithmetic sequences (OEIS
 *            A077264), where each row i has step = block number m (1,2,3,…)
 *            and start = i minus the total number of rows preceding that
 *            block. The sequence a(n) is obtained by traversing this table on
 *            its antidiagonals
 * Range:     u8 n <= 83, u16 n <= 83, u32 n <= 83, u64 n <= 83, u128 n <= 83,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

static idx_t isqrt_u128(unsigned __int128 x) {
    /* floor(sqrt(x)) with 128‑bit arithmetic, result fits in idx_t. */
    idx_t lo = 0;
    idx_t hi = ((idx_t)1u << 63);   /* 2^63 is an upper bound for the sqrt */
    while (lo <= hi) {
        idx_t mid = lo + (hi - lo) / 2;
        unsigned __int128 sq = (unsigned __int128)mid * mid;
        if (sq <= x) {
            lo = mid + 1;
        } else {
            if (mid == 0) break;
            hi = mid - 1;
        }
    }
    return hi;
}

seq_t a(idx_t n) {
    /* --- diagonal index d (where n lies) --- */
    unsigned __int128 t = (unsigned __int128)n * 8 + 1;
    idx_t d = (isqrt_u128(t) - 1) / 2;

    /* --- position inside the diagonal --- */
    idx_t diag_start = d * (d + 1) / 2;
    idx_t r = n - diag_start;          /* 0 ≤ r ≤ d */

    /* --- table coordinates (i, j) --- */
    idx_t i = d - r;   /* row index   */
    idx_t j = r;       /* column index */

    /* --- determine the block number for row i --- */
    t = (unsigned __int128)i * 8 + 1;
    idx_t bi = (isqrt_u128(t) - 1) / 2;   /* bi such that bi(bi+1)/2 ≤ i */
    idx_t step = bi + 1;                  /* step for this row */

    /* --- start value of the row --- */
    idx_t prev_rows = bi * (bi + 1) / 2;   /* rows before this block */
    idx_t start = i - prev_rows;           /* start = i - prev_rows */

    /* --- compute the value at (i, j) --- */
    return (seq_t)start + (seq_t)step * (seq_t)j;
}
