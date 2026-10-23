/*
 * A141671  Triangle T(n, k) = n/k if n mod k = 0, otherwise T(n, k) = 0, with
 *          T(n, 0) = n+1, read by rows.
 * https://oeis.org/A141671
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy, tabl
 * Formula:   a(n) = T(r,k) where r and k are derived from n by * reading the
 *            triangle T(r,k) = (r/k if r mod k == 0) else 0, * T(r,0) = r+1,
 *            with rows concatenated starting from r=0. * * Implementation:
 *            map linear index n to (r,k) where * start(r) = r*(r+1)/2 (first
 *            index of row r) * k = n - start(r) * Then c...
 * Range:     u8 n <= 5150, u16 n <= 5150, u32 n <= 5150, u64 n <= 5150,
 *            u128 n <= 5150, big any n
 * Helpers:   start_index_of
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include "../utils/start_index_of.h"

static inline idx_t row_of(idx_t n) {
    /* Find largest r such that r*(r+1)/2 <= n */
    idx_t low = 0, high = n;
    while (low < high) {
        idx_t mid = low + (high - low + 1) / 2;
        unsigned __int128 start = (unsigned __int128)mid * (mid + 1) / 2;
        if (start <= n) {
            low = mid;
        } else {
            high = mid - 1;
        }
    }
    return low;
}

seq_t a(idx_t n) {
    /* 1. Determine the row r containing index n */
    idx_t r = row_of(n);

    /* 2. Compute start index of this row */
    idx_t start = start_index_of(r);

    /* 3. Column k within the row */
    idx_t k = n - start;

    /* 4. Compute triangle value */
    if (k == 0) {
        return (seq_t)(r + 1);
    } else {
        if (r % k == 0) {
            return (seq_t)(r / k);
        } else {
            return (seq_t)0;
        }
    }
}
