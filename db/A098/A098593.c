/*
 * A098593  A triangle of Krawtchouk coefficients.
 * https://oeis.org/A098593
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, sign, tabl
 * Formula:   not stated
 * Range:     i8 n <= 155, i16 n <= 600, i32 n <= 1274, i64 n <= 1274,
 *            i128 n <= 1274, big any n
 * Validated: standard, pass, 113 terms through n = 1272 (2026-11-03)
 * Helpers:   mem
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  A098593:  Triangle of Krawtchouk coefficients
 *  T(0,0)=1, T(n,0)=T(n,n)=1
 *  T(n,k)=T(n-1,k)+T(n-1,k-1)-2*T(n-2,k-1)  (n≥2, 1≤k≤n-1)
 *
 *  a(n) is the entry T(r,c) where r is the row such that
 *    r(r+1)/2 ≤ n < (r+1)(r+2)/2
 *    c = n - r(r+1)/2
 *
 *  The function keeps a cached triangle, grown lazily to the requested
 *  row.  Memory is allocated with malloc/realloc and explicitly cast to
 *  the proper pointer types so the code compiles in both C11 and
 *  C++17.
 *
 *  All arithmetic uses the seq_t type supplied by the build; no
 *  multidigit or floating‑point operations are needed, so the routine
 *  is exact for every term that fits in seq_t.
 */

#include <stdlib.h>   /* malloc, realloc */
#include "../utils/mem.h"

/*----- helper: floor(sqrt(1+8*n)) via binary search -----*/
static inline idx_t isqrt8plus1(idx_t n)
{
    idx_t low = 0, high = 1;
    /* enlarge high until high*high >= 1+8*n */
    while (high * high <= 1 + 8 * n) {
        high <<= 1;
    }
    while (low < high) {
        idx_t mid = (low + high + 1) >> 1;
        if (mid * mid <= 1 + 8 * n)
            low = mid;
        else
            high = mid - 1;
    }
    return low;              /* floor(sqrt(1+8*n)) */
}

/*----- static cache of the triangle -----*/
static seq_t **triangle = (seq_t **)NULL;   /* array of rows */
static size_t  cached_rows = 0;             /* number of rows stored */

/*----- main function -----*/
seq_t a(idx_t n)
{
    /* find row r and column c for index n */
    idx_t r = (isqrt8plus1(n) - 1) / 2;    /* floor((sqrt(1+8n)-1)/2) */
    idx_t offset = r * (r + 1) / 2;
    idx_t c = n - offset;

    /* grow the triangle if needed */
    if (cached_rows <= (size_t)r) {
        size_t new_rows = r + 1;
        /* reallocate the array of row pointers */
        triangle = (seq_t **)((size_t)realloc(triangle,
                     new_rows * sizeof(seq_t *)));
        if (triangle == NULL) {              /* allocation failed */
            /* fallback: return 0 (should not happen in tests) */
            return (seq_t)0;
        }
        /* build rows [cached_rows .. r] */
        for (size_t i = cached_rows; i < new_rows; ++i) {
            size_t len = i + 1;
            /* allocate this row */
            triangle[i] = mem_new(seq_t, len);
            if (triangle[i] == NULL) {
                /* allocation failed – return 0 */
                return (seq_t)0;
            }
            /* fill the row */
            for (size_t k = 0; k < len; ++k) {
                seq_t val;
                if (k == 0 || k == i) {           /* boundary */
                    val = (seq_t)1;
                } else if (i >= 2) {              /* recurrence */
                    seq_t t1 = triangle[i - 1][k];
                    seq_t t2 = triangle[i - 1][k - 1];
                    seq_t t3 = triangle[i - 2][k - 1];
                    val = t1 + t2 - (seq_t)(2 * t3);
                } else {                          /* i == 1, k==1 handled above */
                    val = (seq_t)1;
                }
                triangle[i][k] = val;
            }
        }
        cached_rows = new_rows;
    }

    return triangle[r][c];
}
