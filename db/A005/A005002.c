/*
 * A005002  Number of rhyme schemes (see reference for precise definition).
 * https://oeis.org/A005002
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, nice
 * Formula:   a(n) = 2*a(n-1) + Bell(n), a(1) = 1; Bell numbers from the Bell
 *            triangle
 * Range:     u8 n <= 5, u16 n <= 9, u32 n <= 15, u64 n <= 25, u128 n <= 42,
 *            big any n
 * Helpers:   alloc_zero, grow_zero
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdbool.h>
#include "../utils/alloc_zero.h"
#include "../utils/grow_zero.h"

static seq_t *a_cache = NULL;   /* a_cache[i] = a(i) for 1 <= i <= a_done */
static idx_t a_cap = 0;
static idx_t a_done = 0;

static seq_t *row = NULL;       /* current Bell triangle row r, length r+1 */
static seq_t *nrow = NULL;
static idx_t row_cap = 0;
static idx_t row_r = 0;

static void ensure_row_cap(idx_t len) {
    if (row_cap >= len) return;
    idx_t ncap = row_cap ? row_cap : 16;
    while (ncap < len) ncap *= 2;
    row = grow_zero(row, row_cap, ncap);
    nrow = grow_zero(nrow, row_cap, ncap);
    row_cap = ncap;
}

/* advance from Bell triangle row r to row r+1 */
static void advance_row(void) {
    idx_t r = row_r;
    ensure_row_cap(r + 2);
    nrow[0] = row[r];
    for (idx_t i = 1; i <= r + 1; i++)
        nrow[i] = nrow[i - 1] + row[i - 1];
    seq_t *t = row;
    row = nrow;
    nrow = t;
    row_r = r + 1;
}

seq_t a(idx_t n) {
    if (a_cache == NULL) {
        ensure_row_cap(1);
        row[0] = 1;
        row_r = 0;
        a_cap = 16;
        a_cache = alloc_zero(a_cap);
        a_cache[1] = 1;
        a_done = 1;
    }
    if (n <= a_done) return a_cache[n];
    if (n + 1 > a_cap) {
        idx_t ncap = a_cap;
        while (ncap < n + 1) ncap *= 2;
        a_cache = grow_zero(a_cache, a_cap, ncap);
        a_cap = ncap;
    }
    for (idx_t m = a_done + 1; m <= n; m++) {
        /* Bell(m) is the last element of row m-1 */
        while (row_r < m - 1) advance_row();
        a_cache[m] = (seq_t)2 * a_cache[m - 1] + row[row_r];
    }
    a_done = n;
    return a_cache[n];
}
