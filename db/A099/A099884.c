/*
 * A099884  XOR difference triangle of the powers of 2, read by rows; Square
 *          array A(row,col): A(0,col) = 2^col, A(row,col) = A048724(A(row-1,
 *          col)) for row > 0, read by descending antidiagonals.
 * https://oeis.org/A099884
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, tabl
 * Formula:   not stated
 * Range:     u8 n <= 35, u16 n <= 135, u32 n <= 527, u64 n <= 1080,
 *            u128 n <= 1080, big any n
 * Helpers:   diag_sum_floor
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* 
 * Formula: a(n) = A(t, c) where:
 *   s = floor((sqrt(8*n+1)-1)/2)   // sum of indices on antidiagonal
 *   t = n - s*(s+1)/2              // row index
 *   c = s - t                      // column index
 *   A(t, c) = F_t * 2^c
 *   F_0 = 1
 *   F_i = F_{i-1} XOR (F_{i-1} << 1)   // main diagonal (A001317)
 */

#include <stdint.h>
#include <stdlib.h>
#include "../utils/diag_sum_floor.h"

/* Memoized array of F_t values (main diagonal) */
static seq_t *F_cache = NULL;
static size_t F_size = 0;   /* number of values stored (i.e., F_cache[0..F_size-1] exist) */
static size_t F_cap = 0;    /* allocated capacity */

static void ensure_F_capacity(size_t needed) {
    if (needed < F_cap)
        return;
    size_t new_cap = (F_cap == 0) ? 4 : F_cap << 1;
    while (new_cap <= needed)
        new_cap <<= 1;
    seq_t *ptr = (seq_t *)realloc(F_cache, new_cap * sizeof(seq_t));
    if (ptr == NULL) { /* allocation failed; stop execution */
        exit(EXIT_FAILURE);
    }
    F_cache = ptr;
    F_cap = new_cap;
}

static seq_t get_F(size_t t) {
    if (t >= F_size) {
        ensure_F_capacity(t + 1);
        if (t == 0) {
            F_cache[0] = (seq_t)1;
            F_size = 1;
        }
        for (size_t i = F_size; i <= t; ++i) {
            F_cache[i] = F_cache[i - 1] ^ (F_cache[i - 1] << 1);
        }
        F_size = t + 1;
    }
    return F_cache[t];
}

seq_t a(idx_t n) {
    /* Find diagonal index s */
    idx_t s = diag_sum_floor(n);
    /* Compute starting index of this diagonal */
    unsigned __int128 tri = (unsigned __int128)s * (s + 1) / 2;
    idx_t t = n - (idx_t)tri;   /* row index within diagonal */
    idx_t c = s - t;            /* column index within diagonal */

    /* Retrieve F_t */
    seq_t Ft = get_F((size_t)t);

    /* Shift by c, guarding against overflow */
    size_t bits = sizeof(seq_t) * 8;
    if ((idx_t)c >= (idx_t)bits)
        return (seq_t)0;   /* value would not fit; return 0 as safe placeholder */
    return Ft << c;
}
