/*
 * A193728  Triangular array: the fusion of polynomial sequences P and Q given
 *          by p(n,x) = (x+2)^n and q(n,x) = (2*x+1)^n.
 * https://oeis.org/A193728
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, tabl
 * Formula:   not stated
 * Range:     u8 n <= 10, u16 n <= 29, u32 n <= 94, u64 n <= 331,
 *            u128 n <= 1190, big any n
 * Helpers:   grow_seq
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* OEIS A193728:  Triangular array flattening
   The triangle T(n,k) is defined by
       T(0,0) = 1 ,   T(1,0) = 2 ,   T(1,1) = 1
       T(n,0)   = 4 * T(n-1,0)
       T(n,n)   = 3 * T(n-1,n-1)
       T(n,k)   = 3 * T(n-1,k-1) + 4 * T(n-1,k)   (1 ≤ k ≤ n-1)
   The sequence a(n) is the row‑major flattening of this triangle.
   a(0)=1, a(1)=2, a(2)=1, a(3)=8, a(4)=10, … */

#include <stdint.h>
#include "../utils/grow_seq.h"

seq_t a(idx_t n)
{
    /* Determine the row r that contains the n-th term */
    idx_t r = 0;
    while ((r + 1) * (r + 2) / 2 <= n) {
        ++r;
    }

    /* Column index within the row */
    idx_t k = n - r * (r + 1) / 2;

    /* Static storage for the triangle */
    static seq_t *tri = NULL;
    static size_t tri_alloc = 0;   /* number of seq_t elements allocated */
    static idx_t max_row = (idx_t)-1;   /* last fully computed row */

    /* Ensure we have enough space for row r */
    size_t needed = (size_t)(r + 1) * (r + 2) / 2;
    if (tri_alloc < needed) {
        size_t new_cap = needed * 3 / 2 + 1;
        grow_seq(&tri, &tri_alloc, new_cap);
    }

    /* Initialise the triangle if it hasn't been started yet */
    if (max_row == (idx_t)-1) {
        tri[0] = (seq_t)1;   /* T(0,0) */
        max_row = 0;
    }

    /* Compute new rows up to r */
    while (max_row < r) {
        idx_t i = max_row + 1;                      /* next row index to compute */
        size_t off_prev = (size_t)i * (i - 1) / 2;  /* start of row i-1 */
        size_t off_cur  = (size_t)i * (i + 1) / 2;  /* start of row i  */

        if (i == 1) {
            tri[off_cur + 0] = (seq_t)2; /* T(1,0) */
            tri[off_cur + 1] = (seq_t)1; /* T(1,1) */
        } else {   /* i >= 2 */
            /* first element of the row */
            tri[off_cur + 0] = (seq_t)4 * tri[off_prev + 0];

            /* middle elements */
            for (size_t j = 1; j < i; ++j) {
                tri[off_cur + j] =
                    (seq_t)3 * tri[off_prev + (j - 1)] +
                    (seq_t)4 * tri[off_prev + j];
            }

            /* last element of the row */
            tri[off_cur + i] =
                (seq_t)3 * tri[off_prev + (i - 1)];
        }

        max_row = i;
    }

    /* Return the requested element */
    size_t ans_idx = (size_t)r * (r + 1) / 2 + k;  /* equals n */
    return tri[ans_idx];
}
