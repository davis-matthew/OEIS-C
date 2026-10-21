/*
 * A080045  Denominators of triangular array: T(n,1)=T(n,n)=1/n and
 *          T(n,k)=T(n-1,k-1)+t(n-1,k), 1<k<n.
 * https://oeis.org/A080045
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, tabl, frac
 * Formula:   not stated
 * Range:     u8 n <= 22, u16 n <= 38, u32 n <= 68, u64 n <= 68, u128 n <= 68,
 *            big any n
 * Helpers:   gcd
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Recurrence: Denominators of the triangular array
   T(n,1)=T(n,n)=1/n
   T(n,k)=T(n-1,k-1)+T(n-1,k)      (1<k<n)
   a(n) is the denominator of the n-th element when the triangle
   is traversed row by row.  */
    
#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>
#include "../utils/gcd.h"

seq_t a(idx_t n) {
    if (n == 0) return 0;          /* out-of-range guard */

    /* static cache of numerators and denominators */
    static seq_t *num_cache = NULL;
    static seq_t *den_cache = NULL;
    static idx_t   max_row   = 0;     /* last row that has been built */
    static size_t  max_elems = 0;     /* current size of the cached arrays */

    /* find the row r so that r*(r+1)/2 >= n */
    idx_t r = 0;
    while ((r * (r + 1)) / 2 < n) r++;

    /* build new rows up to r if needed */
    if (r > max_row) {
        size_t new_elems = (size_t)r * (r + 1) / 2;
        if (new_elems > max_elems) {
            seq_t *new_num = (seq_t *)realloc(num_cache, new_elems * sizeof(seq_t));
            seq_t *new_den = (seq_t *)realloc(den_cache, new_elems * sizeof(seq_t));
            if (!new_num || !new_den) exit(EXIT_FAILURE);
            num_cache = new_num;
            den_cache = new_den;
            max_elems = new_elems;
        }

        /* build each row from max_row+1 up to r */
        for (idx_t i = max_row + 1; i <= r; ++i) {
            size_t start_cur  = (size_t)(i - 1) * i / 2;   /* first index of row i */
            size_t start_prev = 0;
            if (i > 1) start_prev = (size_t)(i - 2) * (i - 1) / 2; /* first index of row i-1 */

            for (idx_t k = 1; k <= i; ++k) {
                size_t idx = start_cur + (k - 1);

                if (k == 1 || k == i) {            /* boundary entries */
                    num_cache[idx] = (seq_t)1;
                    den_cache[idx] = (seq_t)i;
                } else {                           /* internal entry */
                    seq_t n_left  = num_cache[start_prev + (k - 2)];
                    seq_t d_left  = den_cache[start_prev + (k - 2)];
                    seq_t n_right = num_cache[start_prev + (k - 1)];
                    seq_t d_right = den_cache[start_prev + (k - 1)];

                    seq_t n_new = n_left * d_right + n_right * d_left;
                    seq_t d_new = d_left * d_right;

                    seq_t g = gcd(n_new, d_new);
                    n_new /= g;
                    d_new /= g;

                    num_cache[idx] = n_new;
                    den_cache[idx] = d_new;
                }
            }
        }
        max_row = r;
    }

    return den_cache[n - 1];   /* zero‑based index */
}
