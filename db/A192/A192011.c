/*
 * A192011  Let P(0,x) = -1, P(1,x) = 2*x, and P(n,x) = x*P(n-1,x) - P(n-2,x)
 *          for n > 1. This sequence is the triangle of polynomial
 *          coefficients in order of decreasing exponents.
 * https://oeis.org/A192011
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign, easy, tabl
 * Formula:   not stated
 * Range:     i8 n <= 96, i16 n <= 362, i32 n <= 495, i64 n <= 495,
 *            i128 n <= 495, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A192011: triangle of polynomial coefficients.
   Recurrence:
     T(0,0) = –1
     T(n,0) = 2        (n ≥ 1)
     T(n,k) = T(n−1,k) – T(n−2,k−2)   (0 ≤ k ≤ n, n ≥ 1)
   a(n) is the element with linear index n in this triangle (row‑major).  */

#include <stdlib.h>
#include <string.h>
#include <stdint.h>

#ifdef __cplusplus
#include <new>
#endif

/* Find the row r such that r*(r+1)/2 ≤ n < (r+1)*(r+2)/2.  */
static inline idx_t
row_for_idx(idx_t n)
{
    idx_t lo = 0, hi = 1;
    while ( (((__int128)hi * (hi + 1)) / 2) <= n ) {
        hi <<= 1;
        if (hi == 0) {                 /* overflow: use maximum value */
            hi = (idx_t)-1;
            break;
        }
    }
    while (lo < hi) {
        idx_t mid = (lo + hi + 1) >> 1;
        if ( (((__int128)mid * (mid + 1)) / 2) <= n )
            lo = mid;
        else
            hi = mid - 1;
    }
    return lo;
}

seq_t
a(idx_t n)
{
    /* Static cache keeps all values computed so far.  */
    static seq_t *cache     = NULL;
    static idx_t  capacity  = 0;   /* number of elements in cache   */
    static idx_t  next_row  = 0;   /* index of next row to compute   */

    /* Determine the row that contains index n.  */
    idx_t r_end = row_for_idx(n);

    /* Compute needed capacity: number of elements up to and including r_end. */
    idx_t needed_cap = (idx_t)(((__int128)(r_end + 1) * (r_end + 2)) / 2);

    /* Resize cache if necessary.  */
    if (needed_cap > capacity) {
#ifdef __cplusplus
        seq_t *new_cache = new seq_t[needed_cap];
        if (cache) {
            /* Copy old values. */
            for (idx_t i = 0; i < capacity; ++i)
                new_cache[i] = cache[i];
            delete [] cache;
        }
        cache = new_cache;
#else
        seq_t *new_cache = (seq_t *)realloc(cache,
                                            (size_t)needed_cap * sizeof(seq_t));
        if (!new_cache) abort();
        cache = new_cache;
#endif
        capacity = needed_cap;
    }

    /* Compute and store missing rows up to r_end.  */
    for (idx_t r = next_row; r <= r_end; ++r) {
        idx_t offset = (idx_t)(((__int128)r * (r + 1)) / 2);
        if (r == 0) {
            cache[0] = (seq_t)-1;
            continue;
        }
        for (idx_t k = 0; k <= r; ++k) {
            if (k == 0) {
                cache[offset] = (seq_t)2;
            } else {
                seq_t term1 = 0, term2 = 0;
                if (k <= r - 1) {
                    idx_t offset1 = (idx_t)(((__int128)(r - 1) * r) / 2);
                    term1 = cache[offset1 + k];
                }
                if (k >= 2) {
                    idx_t offset2 = (idx_t)(((__int128)(r - 2) * (r - 1)) / 2);
                    term2 = cache[offset2 + (k - 2)];
                }
                cache[offset + k] = term1 - term2;
            }
        }
    }

    next_row = r_end + 1;
    return cache[n];
}
