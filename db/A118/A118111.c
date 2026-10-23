/*
 * A118111  Binary representation of n-th iteration of the Rule 190 elementary
 *          cellular automaton starting with a single black cell.
 * https://oeis.org/A118111
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, tabf
 * Formula:   not stated
 * Range:     u8 n <= 9999, u16 n <= 9999, u32 n <= 9999, u64 n <= 9999,
 *            u128 n <= 9999, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* 
 * Formula: a(n) is the nth bit of the infinite binary string obtained by
 * concatenating the rows of Rule 190 elementary cellular automaton 
 * starting with a single black cell.  The entry a(n) is 0 or 1.
 * This code computes the required row by iterating the automaton up to
 * row r, where r = floor(sqrt(n)), which is the row containing the
 * nth bit (since the first r rows contain r² bits).
 * Time complexity: O(√n); space: O(√n).
 */

#include <stdlib.h>
#include <stdint.h>
#include <math.h>

/* Integer square‑root; returns floor(sqrt(x)).  Uses 128‑bit arithmetic
 * to avoid overflow. */
static inline idx_t integer_sqrt(idx_t x) {
    /* Initial approximation via double sqrt */
    double d = sqrt((double)x);
    idx_t r = (idx_t)d;
    /* Correct for possible rounding errors */
    while (( (__uint128_t)(r + 1) * (r + 1) ) <= x) ++r;
    while (( (__uint128_t)r * r ) > x) --r;
    return r;
}

/*
 * Return the nth term of the sequence A118111.
 */
seq_t a(idx_t n) {
    /* Identify the row that contains position n */
    idx_t r = integer_sqrt(n);
    idx_t pos = n - r * r;          /* 0‑based index within row r */

    /* Initial row (r = 0) */
    uint8_t *prev = (uint8_t *)malloc((size_t)1);
    if (!prev) return (seq_t)0;     /* out of memory – unlikely */
    prev[0] = 1;
    idx_t prev_len = 1;
    idx_t offset = 0;               /* coordinate of prev[0] */

    /* Build rows 1..r */
    for (idx_t step = 1; step <= r; ++step) {
        idx_t new_offset = offset - 1;
        idx_t new_len = 2 * step + 1;
        uint8_t *curr = (uint8_t *)malloc((size_t)new_len);
        if (!curr) {
            free(prev);
            return (seq_t)0;         /* out of memory */
        }

        for (idx_t j = 0; j < new_len; ++j) {
            /* Coordinate of the current new cell */
            idx_t coord = new_offset + j;

            /* Offsets of the left, center and right neighbors */
            idx_t l_coord = coord - 1;
            idx_t c_coord = coord;
            idx_t r_coord = coord + 1;

            /* Translate to indices in the previous array */
            idx_t l_idx = l_coord - (idx_t)offset;
            idx_t c_idx = c_coord - (idx_t)offset;
            idx_t rr_idx = r_coord - (idx_t)offset;

            /* Neighbor values (0 if out of bounds) */
            uint8_t l = (l_idx < prev_len && l_idx >= 0) ? prev[l_idx] : 0;
            uint8_t c = (c_idx < prev_len && c_idx >= 0) ? prev[c_idx] : 0;
            uint8_t rr = (rr_idx < prev_len && rr_idx >= 0) ? prev[rr_idx] : 0;

            /* Rule 190: output 0 only for patterns 110 or 000, otherwise 1 */
            curr[j] = ((l == 1 && c == 1 && rr == 0) ||
                       (l == 0 && c == 0 && rr == 0)) ? 0 : 1;
        }

        free(prev);
        prev = curr;
        prev_len = new_len;
        offset = new_offset;
    }

    uint8_t bit = prev[pos];
    free(prev);
    return (seq_t)bit;
}
