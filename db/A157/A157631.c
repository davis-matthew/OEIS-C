/*
 * A157631  A general recursion triangle with third part a power triangle:m=4;
 *          Power triangle: f(n,k,m)=If[n*k*(n - k) == 0, 1, n^m - (k^m + (n -
 *          k)^m)]; Recursion: A(n,k,m)=(m*(n - k) + 1)*A(n - 1, k - 1, m) +
 *          (m*k + 1)*A(n - 1, k, m) + m*f(n, k, m)*A(n - 2, k - 1, m).
 * https://oeis.org/A157631
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, tabl, uned
 * Formula:   not stated
 * Range:     u8 n <= 6, u16 n <= 11, u32 n <= 30, u64 n <= 37, u128 n <= 37,
 *            big any n
 * Helpers:   offset_row, pos
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Recurrence for m = 4:  A(0,0)=1,  A(n,0)=A(n,n)=1
   A(n,k) = (4*(n-k)+1) A(n-1,k-1) + (4*k+1) A(n-1,k) + 4*f(n,k) A(n-2,k-1)
   where  f(n,k) = 1                         if n*k*(n-k)==0
               n^4 - (k^4 + (n-k)^4)       otherwise
   a(n) is the flattened triangle (row‑wise) with offset 0. */

#include <stdlib.h>
#include <stdint.h>
#include "../utils/offset_row.h"
#include "../utils/pos.h"

/* cache for A(n,k) values */
static seq_t *cache = NULL;
static idx_t cache_size  = 0;  /* number of elements allocated */
static idx_t max_row_done = 0; /* last row fully computed (row 0 scheduled) */

/* n^4 as seq_t */
static inline seq_t pow4(idx_t v)
{
    seq_t x = (seq_t)v;
    x *= x;
    x *= x;
    return x;
}

/* Power triangle value */
static inline seq_t f_val(idx_t n, idx_t k)
{
    if (n == 0 || k == 0 || k == n) return (seq_t)1;
    seq_t tn  = pow4(n);
    seq_t tk  = pow4(k);
    seq_t tnk = pow4(n - k);
    return tn - (tk + tnk);
}

/* Return already cached value (no bounds check, caller ensures k <= n) */
static inline seq_t get_A(idx_t n, idx_t k)
{
    return cache[pos(n, k)];
}

/* Ensure rows 0…r_needed are fully computed */
static void ensure_precompute(idx_t r_needed)
{
    /* First time: allocate space for row 0 */
    if (cache == NULL) {
        cache_size = 1;
        cache = (seq_t *)malloc(cache_size * sizeof(seq_t));
        cache[0] = (seq_t)1;       /* A(0,0) = 1 */
        max_row_done = 0;
    }

    if (r_needed <= max_row_done) return;

    /* Allocate space for rows until r_needed */
    idx_t new_size = offset_row(r_needed + 1);
    if (new_size > cache_size) {
        seq_t *tmp = (seq_t *)realloc(cache, new_size * sizeof(seq_t));
        if (tmp == NULL) { /* allocation failed */
            free(cache);
            cache = NULL;
            cache_size = 0;
            max_row_done = 0;
            return;
        }
        cache = tmp;
        cache_size = new_size;
    }

    /* Build rows from (max_row_done+1) up to r_needed */
    for (idx_t n = max_row_done + 1; n <= r_needed; ++n) {
        for (idx_t k = 0; k <= n; ++k) {
            seq_t val;
            if (k == 0 || k == n) {
                val = (seq_t)1;
            } else {
                seq_t term1 = (seq_t)(4 * (n - k) + 1) * get_A(n - 1, k - 1);
                seq_t term2 = (seq_t)(4 * k + 1) * get_A(n - 1, k);
                seq_t term3 = (seq_t)4 * f_val(n, k) * get_A(n - 2, k - 1);
                val = term1 + term2 + term3;
            }
            cache[pos(n, k)] = val;
        }
    }
    max_row_done = r_needed;
}

/* Locate row r such that offset_row(r) <= idx < offset_row(r+1) */
static idx_t row_of(idx_t idx)
{
    idx_t r = 0;
    while (offset_row(r + 1) <= idx) ++r;
    return r;
}

/* Public function required by the build system */
seq_t a(idx_t n)
{
    /* Special‑case for n==0 to avoid repeated allocation checks */
    if (n == 0) return (seq_t)1;

    idx_t r = row_of(n);
    idx_t k = n - offset_row(r);
    ensure_precompute(r);
    return get_A(r, k);
}
