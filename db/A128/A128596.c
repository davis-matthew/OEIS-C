/*
 * A128596  Triangle, read by rows, of coefficients of q^(nk) in the q-analog
 *          of the even double factorials: T(n,k) = [q^(nk)] Product_{j=1..n}
 *          (1-q^(2j))/(1-q) for n>0, with T(0,0)=1.
 * https://oeis.org/A128596
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, tabl
 * Formula:   a(n) = coefficient of q^(n*k) in the product Π_{j=1..n}
 *            (1-q^(2j))/(1-q) read row-wise
 * Range:     u8 n <= 16, u16 n <= 37, u32 n <= 50, u64 n <= 50, u128 n <= 50,
 *            big any n
 * Validated: partial, pass, 51 terms through n = 50 (2026-11-03)
 * Helpers:   mem
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <math.h>
#include <stdint.h>
#include "../utils/mem.h"

static seq_t *table = NULL;
static size_t table_size = 0;
static idx_t rows_computed = 0;

static seq_t *poly = NULL;
static idx_t poly_len = 0;

/* compute row index from linear index */
static idx_t row_of_index(idx_t n) {
    double tmp = sqrt((double)(8.0 * (long double)n + 1.0));
    idx_t r = (idx_t)((tmp - 1.0) / 2.0);
    while ((r + 1) * (r + 2) / 2 <= n) ++r;
    while (r * (r + 1) / 2 > n) --r;
    return r;
}

/* build all rows up to target_row (inclusive) */
static void compute_rows_upto(idx_t target_row) {
    for (idx_t r = rows_computed + 1; r <= target_row; ++r) {
        idx_t factor_len = 2 * r;
        idx_t new_len = poly_len + factor_len - 1;
        seq_t *new_poly = mem_new(seq_t, new_len);
        for (idx_t i = 0; i < new_len; ++i) new_poly[i] = 0;
        for (idx_t i = 0; i < poly_len; ++i) {
            seq_t val = poly[i];
            if (val == 0) continue;
            for (idx_t j = 0; j < factor_len; ++j) {
                new_poly[i + j] += val;
            }
        }
        mem_free(poly);
        poly = new_poly;
        poly_len = new_len;

        idx_t offset = r * (r + 1) / 2;
        if (table_size < offset + r + 1) {
            size_t new_size = offset + r + 1;
            seq_t *new_table = mem_resize(seq_t, table, new_size);
            if (!new_table) abort();
            table = new_table;
            table_size = new_size;
        }
        for (idx_t k = 0; k <= r; ++k) {
            table[offset + k] = poly[r * k];
        }
        rows_computed = r;
    }
}

seq_t a(idx_t n) {
    /* lazy initialization */
    if (!table) {
        table = mem_new(seq_t, 1);
        table[0] = (seq_t)1;
        table_size = 1;
        rows_computed = 0;
        poly = mem_new(seq_t, 1);
        poly[0] = (seq_t)1;
        poly_len = 1;
    }

    if (n < table_size) return table[n];

    idx_t r = row_of_index(n);
    if (r > rows_computed) compute_rows_upto(r);
    return table[n];
}
