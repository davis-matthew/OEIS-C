/*
 * A118407  Triangle, read by rows, equal to the matrix square of triangle
 *          A118404; also equals the matrix inverse of triangle A118401.
 * https://oeis.org/A118407
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign, tabl
 * Formula:   A118407 is a triangle read by rows with row i (0‑based) and
 *            column j (0 ≤ j ≤ i). If we denote that entry by t[i][j] then
 *            t[i][j] = c[i-j], where (c[n])_{n≥0} is the coefficient sequence
 *            of F(x) = (1+x)^2 / (1+x^2) / (1+2x+2x^2) The sequence c[n]
 *            satisfies the linear recurrence c[0] = 1 c[1] = 0...
 * Range:     i8 n <= 90, i16 n <= 90, i32 n <= 90, i64 n <= 90, i128 n <= 90,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

/* Integer square root of a 64‑bit unsigned integer.
   Returns floor( sqrt(x) ). */
static inline idx_t isqrt_u64(idx_t x)
{
    idx_t r = 0;
    /* use a bit‑wise algorithm (>> 1 each iteration) */
    for (int i = 63; i >= 0; --i) {
        idx_t y = r | ((idx_t)1 << i);
        __int128 prod = (__int128)y * y;
        if (prod <= x) r = y;
    }
    return r;
}

/* Cache of pre‑computed coefficients c[n] */
static seq_t *c_cache = NULL;
static idx_t c_cache_len = 0;

/* Compute coefficients up to index 'max' inclusive */
static void ensure_c(idx_t max)
{
    if (max < c_cache_len) return;
    size_t new_len = (size_t)max + 1;
    seq_t *tmp = (seq_t *)realloc(c_cache, new_len * sizeof(seq_t));
    if (!tmp) return;                /* ignore allocation failure in this context */
    c_cache = tmp;
    for (idx_t i = c_cache_len; i <= max; ++i) {
        seq_t term0 = (i == 0) ? (seq_t)1
                 : (i == 1) ? (seq_t)2
                 : (i == 2) ? (seq_t)1
                 : (seq_t)0;
        seq_t sum = 0;
        if (i >= 1) sum += (seq_t)2 * c_cache[i - 1];
        if (i >= 2) sum += (seq_t)3 * c_cache[i - 2];
        if (i >= 3) sum += (seq_t)2 * c_cache[i - 3];
        if (i >= 4) sum += (seq_t)2 * c_cache[i - 4];
        c_cache[i] = term0 - sum;
    }
    c_cache_len = max + 1;
}

seq_t a(idx_t n)
{
    /* Find the row index r: r = floor( ( sqrt(8*n+1) – 1 ) / 2 ) */
    idx_t sq = isqrt_u64((idx_t)(((__int128)8 * n) + 1));
    idx_t r  = (sq - 1) >> 1;

    /* column index k = n – r*(r+1)/2  */
    idx_t tri = (idx_t)(((__int128)r * (r + 1)) / 2);
    idx_t k    = n - tri;

    /* difference d = r – k, 0 ≤ d ≤ r  */
    idx_t d = r - k;
    ensure_c(d);
    return c_cache[d];
}
