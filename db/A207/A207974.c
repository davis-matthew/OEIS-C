/*
 * A207974  Triangle related to A152198.
 * https://oeis.org/A207974
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, nonn, tabl
 * Formula:   a(n) = T(r, c) where T is the triangle defined by T(n,0) = 1,
 *            T(n,k) = T(n-1, k-1) - (-1)^k * T(n-1, k) for k > 0. The
 *            triangle is listed in row‑major order, so a( n ) = the entry at
 *            index n in that ordering
 * Range:     u8 n <= 196, u16 n <= 644, u32 n <= 2376, u64 n <= 8973,
 *            u128 n <= 9372, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Function implementing a(n) for the OEIS sequence A207974. */
/* External linkage as required. */
#include <stdlib.h>

seq_t a(idx_t n) {
    /* Locate the row (r) and column (c) of the entry at position n. */
    idx_t r = 0;
    while (((r + 1) * (r + 2) / 2) <= n) {
        ++r;
    }
    size_t offset   = (size_t)r * (r + 1) / 2;
    size_t c        = (size_t)(n - offset);

    /* Static storage for the triangle, built lazily. */
    static seq_t *tri      = NULL;
    static idx_t max_row   = 0;
    static size_t tri_len  = 0;

    /* Build or extend the triangle up to row r if necessary. */
    if (tri_len == 0) {
        /* First build: rows 0 .. r. */
        size_t new_len = (size_t)(r + 1) * (r + 2) / 2;
        tri = (seq_t *)calloc(new_len, sizeof(seq_t));
        if (!tri) abort();   /* insufficient memory */

        /* Compute all rows from 0 to r. */
        for (idx_t rr = 0; rr <= r; ++rr) {
            size_t row_start = (size_t)rr * (rr + 1) / 2;
            if (rr == 0) {
                tri[row_start] = (seq_t)1;
                continue;
            }
            size_t prev_start = (size_t)(rr - 1) * rr / 2;
            for (size_t cc = 0; cc <= (size_t)rr; ++cc) {
                if (cc == 0 || cc == (size_t)rr) {
                    tri[row_start + cc] = (seq_t)1;
                } else {
                    if ((cc & 1) == 0) { /* cc even */
                        tri[row_start + cc] =
                            tri[prev_start + cc - 1] - tri[prev_start + cc];
                    } else {             /* cc odd  */
                        tri[row_start + cc] =
                            tri[prev_start + cc - 1] + tri[prev_start + cc];
                    }
                }
            }
        }
        max_row = r;
        tri_len = new_len;
    } else if (r > max_row) {
        /* Extend the triangle to include rows up to r. */
        size_t new_len = (size_t)(r + 1) * (r + 2) / 2;
        seq_t *tmp = (seq_t *)realloc(tri, new_len * sizeof(seq_t));
        if (!tmp) abort();   /* out of memory */
        tri = tmp;

        /* Compute new rows only. */
        for (idx_t rr = max_row + 1; rr <= r; ++rr) {
            size_t row_start = (size_t)rr * (rr + 1) / 2;
            size_t prev_start = (size_t)(rr - 1) * rr / 2;
            for (size_t cc = 0; cc <= (size_t)rr; ++cc) {
                if (cc == 0 || cc == (size_t)rr) {
                    tri[row_start + cc] = (seq_t)1;
                } else {
                    if ((cc & 1) == 0) {
                        tri[row_start + cc] =
                            tri[prev_start + cc - 1] - tri[prev_start + cc];
                    } else {
                        tri[row_start + cc] =
                            tri[prev_start + cc - 1] + tri[prev_start + cc];
                    }
                }
            }
        }
        max_row = r;
        tri_len = new_len;
    }

    return tri[offset + c];
}
