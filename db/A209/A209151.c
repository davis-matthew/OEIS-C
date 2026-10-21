/*
 * A209151  Triangle of coefficients of polynomials u(n,x) jointly generated
 *          with A208337; see the Formula section.
 * https://oeis.org/A209151
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, tabl
 * Formula:   a(n) = coefficient u_r,c-1 of the bivariate polynomial u_r(x)
 *            that satisfies u(1,x) = 1 u(n,x) = u(n‑1,x) + (x+1)·v(n‑1,x)
 *            v(1,x) = 1 v(n,x) = x·u(n‑1,x) + x·v(n‑1,x) + 1 The lookup table
 *            is the flattened triangle of all u_r(x), indexed starting at n =
 *            1
 * Range:     u8 n <= 39, u16 n <= 62, u32 n <= 62, u64 n <= 62, u128 n <= 62,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

#if defined(__cplusplus)
#  define ALLOC_SEQ(p, size)  (p) = new seq_t[(size)]
#  define FREE_SEQ(p)         delete[] (p)
#else
#  define ALLOC_SEQ(p, size)  (p) = (seq_t*)malloc((size) * sizeof(seq_t))
#  define FREE_SEQ(p)         free((p))
#endif

seq_t a(idx_t n)
{
    /* Find row r such that r(r+1)/2 >= n by incremental sum
       (avoids overflow using __int128).                                          */
    __int128 sum = 0;
    idx_t r = 0;
    while (sum < (__int128)n) {
        ++r;
        sum += r;
    }
    /* sum now equals r(r+1)/2, so previous rows sum = sum - r            */
    __int128 prev_sum = sum - r;
    idx_t c = (idx_t)(n - (idx_t)prev_sum);   /* column index in 1‑based form */

    /* Special case: first row */
    if (r == 1) return (seq_t)1;

    /* Allocate coefficient arrays for current row */
    size_t max_len = (size_t)r + 1;
    seq_t *u = NULL;
    seq_t *v = NULL;
    ALLOC_SEQ(u, max_len);
    ALLOC_SEQ(v, max_len);
    /* initialize first row */
    u[0] = (seq_t)1;
    v[0] = (seq_t)1;

    /* Build rows 2 … r */
    for (idx_t i = 2; i <= r; ++i) {
        size_t cur_len = (size_t)i;
        seq_t *u_next = NULL;
        seq_t *v_next = NULL;
        ALLOC_SEQ(u_next, cur_len);
        ALLOC_SEQ(v_next, cur_len);

        /* zero initialization (necessary for seq_t that may not be zero‑initialized) */
        for (size_t k = 0; k < cur_len; ++k) {
            u_next[k] = (seq_t)0;
            v_next[k] = (seq_t)0;
        }

        /* u(i) = u(i‑1) + (x+1)·v(i‑1)  →  coefficient recurrence */
        for (idx_t k = 0; k < (idx_t)cur_len; ++k) {
            seq_t term1 = (k < (i-1)) ? u[k] : (seq_t)0;
            seq_t term2 = (k < (i-1)) ? v[k] : (seq_t)0;
            seq_t term3 = (k > 0)     ? v[k-1] : (seq_t)0;
            u_next[k] = term1 + term2 + term3;
        }

        /* v(i) = x·u(i‑1) + x·v(i‑1) + 1  →  coefficient recurrence */
        v_next[0] = (seq_t)1; /* constant term */
        for (idx_t k = 1; k < (idx_t)cur_len; ++k) {
            v_next[k] = u[k-1] + v[k-1];
        }

        /* free previous row and advance pointer */
        FREE_SEQ(u);
        FREE_SEQ(v);
        u = u_next;
        v = v_next;
    }

    /* c is 1‑based; coefficient index is c‑1 */
    seq_t result = u[(size_t)(c - 1)];

    /* free memory */
    FREE_SEQ(u);
    FREE_SEQ(v);
    return result;
}
