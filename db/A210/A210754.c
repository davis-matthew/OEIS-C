/*
 * A210754  Triangle of coefficients of polynomials v(n,x) jointly generated
 *          with A210753; see the Formula section.
 * https://oeis.org/A210754
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, tabl
 * Formula:   not stated
 * Range:     u8 n <= 18, u16 n <= 52, u32 n <= 52, u64 n <= 52, u128 n <= 52,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* 
 * A210754 – Triangle of coefficients of the polynomials v(n,x).
 *
 * Polynomials:
 *   u(1,x) = 1,   v(1,x) = 1
 *   u(n,x) = (x+1)·u(n‑1,x) + x·v(n‑1,x) + 1
 *   v(n,x) = (x+1)·u(n‑1,x) + (x+1)·v(n‑1,x) + 1  (n ≥ 2)
 *
 * The sequence a(n) is the row‑wise traversal of the coefficients of v(n,x)
 * (offset 1).  For a given n ≥ 1, let r be the unique integer with
 * r(r‑1)/2 < n ≤ r(r+1)/2.  Then a(n) = coefficient of x^{k} in v(r,x),
 * where k = n – r(r‑1)/2 – 1.
 */

#include <stdlib.h>

#ifdef __cplusplus
/* C++ compilers: use new/delete for array allocation */
static inline seq_t *alloc_seq(idx_t len) { return new seq_t[len]; }
static inline void free_seq(seq_t *p) { delete [] p; }
#else
/* C compilers: use malloc/free */
static inline seq_t *alloc_seq(idx_t len) { 
    return (seq_t*)malloc(len * sizeof(seq_t)); 
}
static inline void free_seq(seq_t *p) { free(p); }
#endif

seq_t a(idx_t n)
{
    /* Find the row r whose elements contain the n‑th term. */
    idx_t r = 1;
    while ((r * (r + 1)) / 2 < n) ++r;

    /* Column index (0‑based) within that row. */
    idx_t index = n - (r * (r - 1)) / 2 - 1;

    /* Base row (n = 1). */
    seq_t *u = alloc_seq(1);
    seq_t *v = alloc_seq(1);
    u[0] = 1;
    v[0] = 1;

    /* Build rows 2 … r. */
    for (idx_t row = 2; row <= r; ++row) {
        seq_t *uo = u;
        seq_t *vo = v;
        seq_t *ub = alloc_seq(row); /* new u */
        seq_t *vb = alloc_seq(row); /* new v */

        for (idx_t k = 0; k < row; ++k) {
            /* Helper terms from previous row */
            seq_t u_prev_k   = (k < row - 1) ? uo[k]   : 0;
            seq_t u_prev_k1  = (k > 0)       ? uo[k-1] : 0;
            seq_t v_prev_k1  = (k > 0)       ? vo[k-1] : 0;
            seq_t v_prev_k   = (k < row - 1) ? vo[k]   : 0;
            seq_t add_const  = (k == 0) ? 1 : 0;

            /* New u coefficient */
            ub[k] = u_prev_k + u_prev_k1 + v_prev_k1 + add_const;

            /* New v coefficient */
            vb[k] = u_prev_k + u_prev_k1 + v_prev_k + v_prev_k1 + add_const;
        }

        free_seq(u);
        free_seq(v);
        u = ub;
        v = vb;
    }

    /* Result is the requested coefficient from the final row. */
    seq_t result = v[index];

    free_seq(u);
    free_seq(v);
    return result;
}
