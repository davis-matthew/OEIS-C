/*
 * A144562  Triangle read by rows: T(n, k) = 2*n*k + n + k - 1.
 * https://oeis.org/A144562
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy, tabl
 * Formula:   a(n) = T(row,col) where * row = minimal r with r(r+1)/2 ≥ n *
 *            col = n – (row-1)row/2 * T(n,k)=2*n*k + n + k - 1
 * Range:     u8 n <= 65, u16 n <= 5050, u32 n <= 5050, u64 n <= 5050,
 *            u128 n <= 5050, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

static inline idx_t find_row(idx_t n) {
    idx_t low = 1, high = 1;
    /* Find high such that C(high) ≥ n */
    while (((unsigned __int128)high * (high + 1)) / 2 < n)
        high <<= 1;
    /* Binary search */
    while (low < high) {
        idx_t mid = low + (high - low) / 2;
        unsigned __int128 val = ((unsigned __int128)mid * (mid + 1)) / 2;
        if (val >= n)
            high = mid;
        else
            low = mid + 1;
    }
    return low;
}

seq_t a(idx_t n) {
    if (n == 0) return 0;                 /* safety for offset 1 */
    idx_t r = find_row(n);                /* row index */
    unsigned __int128 prev = ((unsigned __int128)(r - 1) * r) / 2;
    idx_t k = n - (idx_t)prev;            /* column index (1‑based) */
    return (seq_t)(((unsigned __int128)2 * r * k + r + k - 1));
}
