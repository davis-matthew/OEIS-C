/*
 * A204143  Symmetric matrix based on f(i,j)=max(ceiling(i/j),ceiling(j/i)),
 *          by antidiagonals.
 * https://oeis.org/A204143
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, tabl
 * Formula:   not stated
 * Range:     u8 n <= 95, u16 n <= 95, u32 n <= 95, u64 n <= 95, u128 n <= 95,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computes A204143:
 * a(n) = max( ceil(i/j), ceil(j/i) ) where (i,j) is the (i,j)-entry of the
 * infinite symmetric matrix M defined by M[i][j] = max(ceil(i/j), ceil(j/i))
 * and the n-th term is taken along antidiagonals (i+j constant) with
 * i increasing from 1 to i+j-1.  The sequence starts with a(1) = 1. */

#include <stdint.h>

static inline idx_t isqrt_u128(__uint128_t x) {
    /* integer sqrt for 128‑bit numbers, binary search on 64‑bit range */
    idx_t low = 0, high = (idx_t)1ULL << 32;  /* sqrt(2^128) < 2^64 */
    while (low < high) {
        idx_t mid = (low + high + 1) >> 1;
        __uint128_t sq = (__uint128_t)mid * mid;
        if (sq <= x)
            low = mid;
        else
            high = mid - 1;
    }
    return low;
}

static inline idx_t diag_index(idx_t n) {
    /* Find smallest S such that S*(S-1)/2 >= n (S >= 2) */
    idx_t low = 2, high = (idx_t)1ULL << 32;          /* upper bound sufficient for any n <= 2^64-1 */
    while (low < high) {
        idx_t mid = low + (high - low) / 2;
        __uint128_t m = (__uint128_t)mid * (mid - 1) / 2;
        if (m < n)
            low = mid + 1;
        else
            high = mid;
    }
    return low;
}

seq_t a(idx_t n) {
    /* Determine diagonal and position within that diagonal */
    idx_t S = diag_index(n);                          /* diagonal sum i + j */
    __uint128_t M_prev = (__uint128_t)(S - 1) * (S - 2) / 2;  /* terms before this diagonal */
    idx_t pos = n - (idx_t)M_prev - 1;                /* 0‑based position inside diagonal */
    idx_t i = pos + 1;                                /* row index */
    idx_t j = S - i;                                  /* column index */

    /* Compute ceil division values */
    idx_t ceil_i_div_j = (i + j - 1) / j;   /* ceil(i/j) */
    idx_t ceil_j_div_i = (j + i - 1) / i;   /* ceil(j/i) */

    /* Result fits in idx_t; cast to seq_t for return */
    seq_t val = (seq_t)(ceil_i_div_j > ceil_j_div_i ? ceil_i_div_j : ceil_j_div_i);
    return val;
}
