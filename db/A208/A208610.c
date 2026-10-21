/*
 * A208610  Triangle of coefficients of polynomials u(n,x) jointly generated
 *          with A208611; see the Formula section.
 * https://oeis.org/A208610
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, tabl
 * Formula:   OEIS A208610 – triangle of coefficients of polynomials * u(n,x)
 *            and v(n,x) defined by * u(1,x)=1, v(1,x)=1 *
 *            u(n,x)=u(n-1,x)+x*v(n-1,x) * v(n,x)=(x+1)*u(n-1,x)+2x*v(n-1,x)+1
 *            * * The sequence a(n) is obtained by enumerating all
 *            coefficients of * u(n,x) row‑wise: row 1 has 1 coefficient, row
 *            2 has...
 * Range:     u8 n <= 32, u16 n <= 61, u32 n <= 61, u64 n <= 61, u128 n <= 61,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <math.h>
#include <stdlib.h>

#if defined(__cplusplus)
#include <new>
#endif

seq_t a(idx_t n)
{
    if (n == 0) {
        /* Out of offset, return 0 */
        return (seq_t)0;
    }

    /* Find the row r such that T(r-1) < n <= T(r) */
    idx_t r = (idx_t)((sqrt((double)(8.0 * n + 1.0)) - 1.0) / 2.0);
    if (r * (r + 1) / 2 < n) ++r;
    while (r > 0 && (r - 1) * r / 2 >= n) --r;

    /* Column index (0‑based) within the row */
    idx_t k = n - (r - 1) * r / 2 - 1;

    /* Special case: row 1 */
    if (r == 1) {
        return (seq_t)1;
    }

    /* Allocate temporary arrays of size r+1 */
    seq_t *a_cur  = (seq_t*)malloc((r + 1) * sizeof(seq_t));
    seq_t *b_cur  = (seq_t*)malloc((r + 1) * sizeof(seq_t));
    seq_t *a_next = (seq_t*)malloc((r + 1) * sizeof(seq_t));
    seq_t *b_next = (seq_t*)malloc((r + 1) * sizeof(seq_t));

#if defined(__cplusplus)
    for (idx_t i = 0; i <= r; ++i) new (&a_cur[i])  seq_t();
    for (idx_t i = 0; i <= r; ++i) new (&b_cur[i])  seq_t();
    for (idx_t i = 0; i <= r; ++i) new (&a_next[i]) seq_t();
    for (idx_t i = 0; i <= r; ++i) new (&b_next[i]) seq_t();
#endif

    /* Initialise first row (n=1) */
    a_cur[0] = (seq_t)1;
    b_cur[0] = (seq_t)1;
    idx_t prev_size = 1;

    /* Iterate up to the required row r */
    for (idx_t row = 2; row <= r; ++row) {
        idx_t curr_size = row; /* new row length */

        for (idx_t i = 0; i < curr_size; ++i) {
            seq_t a_prev_i   = (i < prev_size) ? a_cur[i]   : (seq_t)0;
            seq_t a_prev_im1 = (i > 0 && i - 1 < prev_size) ? a_cur[i - 1] : (seq_t)0;
            seq_t b_prev_im1 = (i > 0 && i - 1 < prev_size) ? b_cur[i - 1] : (seq_t)0;

            if (i == 0) {
                a_next[0] = a_prev_i;
                b_next[0] = a_prev_i + (seq_t)1;
            } else {
                a_next[i] = a_prev_i + b_prev_im1;
                b_next[i] = a_prev_i + a_prev_im1 + (b_prev_im1 << 1);
            }
        }

        prev_size = curr_size;
        /* swap current and next arrays for the next iteration */
        seq_t *tmp = a_cur;  a_cur = a_next;  a_next = tmp;
        tmp = b_cur;        b_cur = b_next;  b_next = tmp;
    }

    seq_t result = a_cur[k];

    /* Destroy constructed objects if compiled as C++ */
#if defined(__cplusplus)
    for (idx_t i = 0; i <= r; ++i) a_cur[i].~seq_t();
    for (idx_t i = 0; i <= r; ++i) b_cur[i].~seq_t();
    for (idx_t i = 0; i <= r; ++i) a_next[i].~seq_t();
    for (idx_t i = 0; i <= r; ++i) b_next[i].~seq_t();
#endif

    free(a_cur);
    free(b_cur);
    free(a_next);
    free(b_next);

    return result;
}
