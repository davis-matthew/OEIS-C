/*
 * A082185  Triangle read by rows: n-th row contains concatenations of
 *          nonempty subsets of {1, 2, ..., n}, ordered first by size and then
 *          lexicographically.
 * https://oeis.org/A082185
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy, tabf, base
 * Formula:   not stated
 * Range:     u8 n <= 25, u16 n <= 119, u32 n <= 1013, u64 n <= 1013,
 *            u128 n <= 1013, big any n
 * Helpers:   binom_u64, digits_int, pow_seq10
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Algorithm:  a(n) is the nth term of a triangle whose
 * rows enumerate all non–empty subsets of {1,…,r} (for position row r)
 * ordered first by subset size and within each size lexicographically.
 * For the given n, determine its row and position, reconstruct the
 * corresponding subset, then concatenate the elements in decimal
 * (e.g., subset {1,3,4} → 134). */

#include <stdint.h>
#include "../utils/binom_u64.h"
#include "../utils/digits_int.h"
#include "../utils/pow_seq10.h"

/* ---- main sequence function ---- */
seq_t a(idx_t n)
{
    /* find row r such that cumulative_terms(r-1) < n ≤ cumulative_terms(r) */
    idx_t r = 1;
    idx_t cumulative = 1;                /* terms up to row 1 */
    while (cumulative < n) {
        ++r;
        cumulative += (((idx_t)1 << r) - 1);
    }

    /* index within the row (0‑based) */
    idx_t prev_cumulative = cumulative - (((idx_t)1 << r) - 1);
    idx_t offset_within_row = n - prev_cumulative - 1;

    /* determine subset size s */
    idx_t idx_in_row = offset_within_row;
    idx_t s = 1;
    for (idx_t k = 1; k <= r; ++k) {
        idx_t c = binom_u64(r, k);
        if (idx_in_row < c) {
            s = k;
            break;
        }
        idx_in_row -= c;
    }

    /* reconstruct the k‑th subset of size s in lexicographic order */
    idx_t remaining = s;
    idx_t position = idx_in_row;
    idx_t comb[64];          /* r ≤ 64 for any 64‑bit index n */
    idx_t comb_idx = 0;

    for (idx_t element = 1; element <= r && remaining > 0; ++element) {
        idx_t skip = binom_u64(r - element, remaining - 1);
        if (skip <= position) {
            position -= skip;          /* skip this element */
        } else {
            comb[comb_idx++] = element;/* select this element */
            --remaining;               /* one less to pick */
        }
    }

    /* concatenate the selected elements into the final value */
    seq_t value = 0;
    for (idx_t i = 0; i < s; ++i) {
        int d = digits_int(comb[i]);
        value = value * pow_seq10(d) + (seq_t)comb[i];
    }

    return value;
}
