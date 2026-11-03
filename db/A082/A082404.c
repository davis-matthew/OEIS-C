/*
 * A082404  Triangle in which n-th row gives trajectory of n under the map x
 *          -> x/2 if x is even, x -> x-1 if x is odd, stopping when we reach
 *          1.
 * https://oeis.org/A082404
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  easy, nonn, tabf
 * Formula:   a(n) is the n-th element of the flattened triangle defined by
 *            the map x -> x/2 if even, x -> x-1 if odd, stopping at 1
 * Range:     u8 n <= 2562, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Validated: standard, pass, 93 terms through n = 9652 (2026-11-01)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>   /* for size_t */

/* Computes the length of the trajectory for a given row number. */
static inline size_t trajectory_len(idx_t n) {
    size_t len = 1;            /* includes the starting value n itself */
    idx_t x = n;
    while (x > 1) {
        if ((x & 1) == 0)     /* even */
            x >>= 1;
        else                  /* odd */
            x -= 1;
        ++len;
    }
    return len;
}

/* Returns the element at position `pos` (1‑based) in the trajectory of `row`. */
static inline seq_t trajectory_value(idx_t row, idx_t pos) {
    seq_t value = row;
    if (pos == 1)
        return value;
    for (idx_t i = 1; i < pos; ++i) {
        if ((value & 1) == 0)   /* even */
            value >>= 1;
        else                   /* odd */
            value -= 1;
    }
    return value;
}

/* Main accessor for the sequence. */
seq_t a(idx_t n) {
    /* Find the row that contains the n‑th term in the flat list. */
    idx_t cumulative = 0;
    idx_t row = 0;
    while (cumulative < n) {
        ++row;
        cumulative += trajectory_len(row);
    }

    /* Position inside that row. */
    idx_t pos_in_row = n - (cumulative - trajectory_len(row));   /* 1‑based */

    return trajectory_value(row, pos_in_row);
}
