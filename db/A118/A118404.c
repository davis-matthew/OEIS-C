/*
 * A118404  Triangle T, read by rows, where all columns of T are different and
 *          yet all columns of the matrix square T^2 (A118407) are equal; also
 *          equals the matrix inverse of triangle A118400.
 * https://oeis.org/A118404
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign, tabl
 * Formula:   a(n) derived from the generating function of the triangle
 *            A118404. Let T(r,c) be the entry in row r (0‑based), column c
 *            (0‑based). The following recurrence holds: T(r,0) = p(r) –
 *            T(r-1,0), for r ≥ 0 T(r,c) = - (T(r-1,c) + T(r-1,c-1)), for r ≥
 *            1, c ≥ 1 where p(r) is the coefficient of x^r in (1+x)...
 * Range:     i8 n <= 83, i16 n <= 152, i32 n <= 152, i64 n <= 152,
 *            i128 n <= 152, big any n
 * Helpers:   p_r
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include "../utils/p_r.h"

seq_t a(idx_t n)
{
    /* static cache of all computed sequence values */
    static seq_t *cache      = NULL;
    static size_t cache_cap  = 0;
    static size_t cache_size = 0;      /* number of elements stored */
    static idx_t  next_row   = 0;      /* next row index to compute */

    /* grow cache if necessary and compute rows until we have element n */
    while (cache_size <= (size_t)n) {
        idx_t r = next_row;                     /* compute row r (0‑based) */
        /* ensure capacity for r+1 new values */
        if (cache_size + (size_t)(r + 1) > cache_cap) {
            size_t new_cap = cache_cap ? cache_cap * 2 : 16;
            while (cache_size + (size_t)(r + 1) > new_cap)
                new_cap <<= 1;
            seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
            if (!tmp)
                abort();                       /* out of memory */
            cache = tmp;
            cache_cap = new_cap;
        }

        /* index of the first element of the previous row in cache */
        size_t start_prev = (r > 0) ? ((size_t)(r - 1) * (size_t)r) / 2 : 0;

        for (idx_t c = 0; c <= r; ++c) {
            seq_t value;
            if (c == 0) {
                seq_t prev0 = (r == 0) ? 0 : cache[start_prev];
                value = p_r(r) - prev0;
            } else {
                seq_t prev_c = (c <= r - 1) ? cache[start_prev + c] : 0;
                seq_t prev_c1 = cache[start_prev + c - 1];
                value = -(prev_c + prev_c1);
            }
            cache[cache_size++] = value;
        }
        ++next_row;
    }

    return cache[(size_t)n];
}
