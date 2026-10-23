/*
 * A134233  (A007318 * A134082 + A134082 * A007318) - A007318 as infinite
 *          lower triangular matrices.
 * https://oeis.org/A134233
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, tabl, changed
 * Formula:   a(n) = (4*(i−j)+1) * binomial(i,j) where i =
 *            floor((sqrt(8*n+1)−1)/2), j = n − i*(i+1)/2
 * Range:     u8 n <= 23, u16 n <= 65, u32 n <= 65, u64 n <= 65, u128 n <= 65,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Implementation uses binary search for i to avoid floating point and
   computes binomial coefficients by multiplicative formula.   */
#include <stdint.h>
#include <stddef.h>

/* type aliases provided by the build: seq_t and idx_t */

static inline idx_t triangular(idx_t r) {
    /* r*(r+1)/2 computed in 128‑bit to avoid overflow */
    return (idx_t)(((unsigned __int128)r * (r + 1)) / 2);
}

/* return largest i with triangular(i) <= n */
static idx_t find_row(idx_t n) {
    idx_t low = 0, high = 1;
    while (triangular(high) <= n) {
        high <<= 1;          /* exponential growth until high covers n */
    }
    while (low + 1 < high) {
        idx_t mid = low + (high - low) / 2;
        if (triangular(mid) <= n)
            low = mid;
        else
            high = mid;
    }
    return low;
}

/* multiplicative binomial coefficient C(n,k) with k <= n */
static seq_t binom(idx_t n, idx_t k) {
    if (k > n) return (seq_t)0;
    if (k > n - k) k = n - k;
    seq_t r = 1;
    for (idx_t i = 1; i <= k; ++i) {
        r = r * (seq_t)(n - k + i) / (seq_t)i;
    }
    return r;
}

seq_t a(idx_t n) {
    idx_t i = find_row(n);
    idx_t j = n - triangular(i);        /* position in row */
    seq_t comb = binom(i, j);
    seq_t factor = (seq_t)(4 * (i - j) + 1);
    return comb * factor;
}
