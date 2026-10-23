/*
 * A155704  Triangle read by rows where T(m,n)=2*m*n + m + n + 10.
 * https://oeis.org/A155704
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, tabl, easy
 * Formula:   not stated
 * Range:     u8 n <= 65, u16 n <= 5050, u32 n <= 5050, u64 n <= 5050,
 *            u128 n <= 5050, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A155704: Triangle read by rows.
 * T(m,n)=2*m*n + m + n + 10, 1 ≤ n ≤ m.
 * a(n) is obtained by reading rows of T(m,n) one after another.
 * To find the coordinates for a given index n:
 *   Find the smallest m such that m*(m+1)/2 ≥ n.
 *   Let prev = (m-1)*m/2, then column = n - prev.
 * The function returns a(n). */

#include <stdint.h>

static inline idx_t triangular(idx_t m) {
    return m * (m + 1) / 2;
}

/* External function required by the build system. */
seq_t a(idx_t n) {
    if (n == 0) { /* Undefined for offset 1, but return 0 for safety. */
        return (seq_t)0;
    }

    /* Find the row m using doubling + binary search. */
    idx_t low = 1, high = 1;
    while (triangular(high) < n) {
        high <<= 1;
    }
    while (low < high) {
        idx_t mid = low + (high - low) / 2;
        if (triangular(mid) >= n) {
            high = mid;
        } else {
            low = mid + 1;
        }
    }
    idx_t m = low;

    idx_t prev = triangular(m - 1);
    idx_t col = n - prev;          /* column index, 1-based */

    /* Compute the triangle value.  Use seq_t for intermediate arithmetic. */
    seq_t val = (seq_t)2 * (seq_t)m * (seq_t)col
              + (seq_t)m + (seq_t)col + (seq_t)10;
    return val;
}
