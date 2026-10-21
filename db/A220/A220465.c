/*
 * A220465  Reverse reluctant sequence of reverse reluctant sequence A004736.
 * https://oeis.org/A220465
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  easy, nonn, tabl
 * Formula:   a(n) = A004736(A004736(n)) where A004736 is the
 *            reverse‑reluctant sequence of 1,2,3,... A004736(n) produces the
 *            n‑th element of the triangle read row‑wise. The implementation
 *            uses a binary search to locate the row r such that r(r+1)/2 >= n
 * Range:     u8 n <= 9870, u16 n <= 9870, u32 n <= 9870, u64 n <= 9870,
 *            u128 n <= 9870, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>

/* Find the row number r for which r(r+1)/2 >= n */
static idx_t find_row(idx_t n) {
    idx_t low = 1, high = 1;
    /* Fastly increase high until condition holds */
    while ((__int128)high * (high + 1) / 2 < n) {
        high <<= 1;
    }
    /* Binary search the minimal r */
    while (low < high) {
        idx_t mid = low + (high - low) / 2;
        if ((__int128)mid * (mid + 1) / 2 >= n) {
            high = mid;
        } else {
            low = mid + 1;
        }
    }
    return low;
}

/* Compute A004736(n) */
static seq_t A004736_seq(idx_t n) {
    idx_t r = find_row(n);
    idx_t prev = (idx_t)((__int128)r * (r - 1) / 2);
    idx_t k = n - prev;               /* position within the row (1‑based) */
    seq_t val = (seq_t)r - (seq_t)k + 1;
    return val;
}

/* Main sequence */
seq_t a(idx_t n) {
    seq_t first = A004736_seq(n);
    return A004736_seq((idx_t)first);
}
