/*
 * A209141  Triangle of coefficients of polynomials u(n,x) jointly generated
 *          with A209142; see the Formula section.
 * https://oeis.org/A209141
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, tabl
 * Formula:   a(n) is the anti‑diagonal entry of the triangle T defined by
 *            T(0,0)=1, T(n,0)=2*T(n-1,0)+T(n-2,0) (for n≥2),
 *            T(n,k)=2*T(n-1,k)+T(n-1,k-1)+T(n-2,k-1)+T(n-2,k-2) with T(n,k)=0
 *            if k>n or k<0
 * Range:     u8 n <= 22, u16 n <= 53, u32 n <= 53, u64 n <= 53, u128 n <= 53,
 *            big any n
 * Helpers:   alloc_seq
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>
#include "../utils/alloc_seq.h"

seq_t a(idx_t n)
{
    /* find the smallest row r such that 1+2+…+r ≥ n */
    idx_t r = 1;
    while (((r * (r + 1)) / 2) < n)
        ++r;

    /* static storage for the computed triangle */
    static idx_t          current_max_row = 0;   /* greatest row computed so far */
    static seq_t **       triangle        = NULL; /* array of row pointers */
    static size_t         triangle_cap    = 0;    /* capacity of triangle array */

    /* grow the array of rows if needed */
    if (r > current_max_row) {
        size_t new_cap = (size_t)r + 1;           /* indices 0…r usable */
        if (new_cap > triangle_cap) {
            seq_t **tmp = (seq_t**)realloc(triangle, new_cap * sizeof(seq_t*));
            triangle = tmp;
            triangle_cap = new_cap;
        }

        /* compute all new rows */
        for (idx_t i = current_max_row + 1; i <= r; ++i) {
            triangle[i] = alloc_seq((size_t)i + 1);   /* columns 0…i (0 unused) */

            if (i == 1) {                          /* row 1 */
                triangle[i][1] = (seq_t)1;
            } else if (i == 2) {                    /* row 2 */
                triangle[i][1] = (seq_t)2;
                triangle[i][2] = (seq_t)1;
            } else {                                /* rows i ≥ 3 */
                for (idx_t k = 1; k <= i; ++k) {
                    seq_t t1 = (k <= i - 1)                  ? triangle[i - 1][(size_t)k] : (seq_t)0;
                    seq_t t2 = (k >= 2 && k - 1 <= i - 1)    ? triangle[i - 1][(size_t)(k - 1)] : (seq_t)0;
                    seq_t t3 = (k >= 2 && k - 1 <= i - 2)    ? triangle[i - 2][(size_t)(k - 1)] : (seq_t)0;
                    seq_t t4 = (k >= 3 && k - 2 <= i - 2)    ? triangle[i - 2][(size_t)(k - 2)] : (seq_t)0;
                    triangle[i][(size_t)k] = (seq_t)2 * t1 + t2 + t3 + t4;
                }
            }
        }
        current_max_row = r;
    }

    /* position of a(n) inside the row r */
    idx_t prev_sum = ((r - 1) * r) / 2;   /* total terms in rows < r */
    idx_t k        = n - prev_sum;        /* 1‑based within the row */

    return triangle[r][(size_t)k];
}
