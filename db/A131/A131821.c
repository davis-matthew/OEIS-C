/*
 * A131821  Triangle read by rows: row n consists of n followed by (n-2) ones
 *          then n.
 * https://oeis.org/A131821
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, tabl, easy
 * Formula:   not stated
 * Range:     u8 n <= 105, u16 n <= 105, u32 n <= 105, u64 n <= 105,
 *            u128 n <= 105, big any n
 * Helpers:   trailing_ge
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A131821 – Triangle read by rows: 
   row r consists of r, then (r‑2) ones, then r. 
   a(n) = r if n is the first or last element of its row, otherwise 1. */

/* Note: no external helpers are used; binary search determines the row. */

#include <stdint.h>
#include "../utils/trailing_ge.h"

/* Helper: find smallest r such that r*(r+1)/2 >= n.  n >= 1. */
static inline idx_t find_row(idx_t n) {
    /* Quick upper bound. */
    idx_t low = 0;
    idx_t high = 1;
    while (!trailing_ge(high, n)) {
        high <<= 1; /* double until condition holds */
    }
    /* Binary search in [low, high] */
    while (low + 1 < high) {
        idx_t mid = (low + high) / 2;
        if (trailing_ge(mid, n))
            high = mid;
        else
            low = mid;
    }
    return high;
}

seq_t a(idx_t n) {
    if (n == 0) {                /* sequence is 1‑based; behaviour below 1 is undefined */
        return (seq_t)0;
    }

    /* Determine row r and position k within that row. */
    idx_t r = find_row(n);
    /* Compute T(r‑1) = r*(r‑1)/2 safely. */
    __uint128_t prev = (__uint128_t)r * (r - 1);
    prev /= 2;
    idx_t tprev = (idx_t)prev;
    idx_t k = n - tprev;           /* 1‑based column index */

    if (k == 1 || k == r) {
        return (seq_t)r;
    }
    return (seq_t)1;
}
