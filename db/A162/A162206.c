/*
 * A162206  Triangle read by rows in which row n (n >= 1) gives coefficients
 *          in expansion of the polynomial f(n) * Product_{i=1..n-1} f(2i),
 *          where f(k) = (1 - x^k)/(1-x).
 * https://oeis.org/A162206
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, tabf
 * Formula:   not stated
 * Range:     u8 n <= 51, u16 n <= 135, u32 n <= 491, u64 n <= 2059,
 *            u128 n <= 8503, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Triangle read by rows:  
   row n (n≥1) is the coefficients of  
   f(n) * Π_{i=1}^{n-1} f(2i) ,  where f(k) = (1 - x^k)/(1-x).   */

/* NOTE: The algorithm builds rows incrementally and stores a flat list
   of all coefficients seen so far.  For an index n ≤ 10 000 the
   required highest row number is about 24, so all data fit easily into
   memory.  The implementation is strictly within the C11/C++17 rules
   given in the problem statement.  All arithmetic is performed on
   seq_t values; no floating point or external libraries are used. */

#include <stdlib.h>
#include <string.h>

seq_t a(idx_t n)
{
    /* static state across calls */
    static seq_t *flat = NULL;    /* flattened coefficients */
    static size_t flat_len = 0;   /* number of stored entries   */
    static size_t flat_cap = 0;   /* capacity of flat array      */

    static seq_t *poly = NULL;    /* product Π_{i=1}^{m} f(2i)   */
    static size_t poly_len = 0;   /* length of poly              */

    static idx_t current_row = 0; /* number of rows generated so far */

    /* ---------- initialise state on first call ---------- */
    if (flat == NULL) {
        /* poly initially equals f(0) = 1  (empty product) */
        poly_len = 1;
        poly = (seq_t *)malloc(poly_len * sizeof(seq_t));
        poly[0] = 1;
    }

    /* ---------- generate rows until we reach the requested index ---------- */
    while (flat_len < (size_t)n) {
        /* ---- 1. generate next row ---------------------------------- */
        ++current_row;                            /* new row number        */
        size_t row_len = poly_len + current_row - 1; /* convolution length */

        seq_t *row = (seq_t *)calloc(row_len, sizeof(seq_t));
        for (size_t i = 0; i < poly_len; ++i) {
            seq_t v = poly[i];
            for (size_t k = 0; k < (size_t)current_row; ++k) {
                row[i + k] += v;
            }
        }

        /* ---- 2. append the row to the flattened array -------------- */
        if (flat_len + row_len > flat_cap) {
            /* grow capacity exponentially for efficiency               */
            flat_cap = flat_cap ? flat_cap * 2 : 64;
            while (flat_len + row_len > flat_cap) flat_cap *= 2;
            flat = (seq_t *)realloc(flat, flat_cap * sizeof(seq_t));
        }
        memcpy(flat + flat_len, row, row_len * sizeof(seq_t));
        flat_len += row_len;

        free(row);          /* row buffer no longer needed */

        /* ---- 3. update poly for the next iteration ---------------- */
        size_t new_poly_len = poly_len + 2 * current_row - 1;
        seq_t *new_poly = (seq_t *)calloc(new_poly_len, sizeof(seq_t));
        for (size_t i = 0; i < poly_len; ++i) {
            seq_t v = poly[i];
            for (size_t k = 0; k < (size_t)(2 * current_row); ++k) {
                new_poly[i + k] += v;
            }
        }

        free(poly);
        poly = new_poly;
        poly_len = new_poly_len;
    }

    /* ---------- answer the query ------------------------------------- */
    return flat[(size_t)n - 1];
}
