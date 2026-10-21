/*
 * A210560  Triangle of coefficients of polynomials v(n,x) jointly generated
 *          with A210559; see the Formula section.
 * https://oeis.org/A210560
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, tabl
 * Formula:   a(n) is the flattened sequence of coefficients of the
 *            polynomials v(r,x) defined by u(1,x)=1 , v(1,x)=1
 *            u(n,x)=x*u(n-1,x)+x*v(n-1,x)+1 v(n,x)=(x+1)*u(n-1,x)+v(n-1,x)+1
 *            The nth term is the coefficient of x^(k-1) in v(r,x), where r is
 *            the smallest integer with r(r+1)/2 ≥ n
 * Range:     u8 n <= 32, u16 n <= 61, u32 n <= 61, u64 n <= 61, u128 n <= 61,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

#ifdef __cplusplus
#  define SEQ_ALLOC(sz) (new seq_t[(sz)])
#  define SEQ_FREE(p)   delete[] (p)
#else
#  define SEQ_ALLOC(sz) ((seq_t*)malloc((sz)*sizeof(seq_t)))
#  define SEQ_FREE(p)   do { free(p); } while(0)
#endif

seq_t a(idx_t n)
{
    if (n < (idx_t)1) return (seq_t)0;   /* undefined for n<1, harmless */

    /* find the row r such that r(r+1)/2 >= n */
    size_t r = 1;
    size_t cum = 1;   /* cumulative count of terms up to current row */
    while (cum < (size_t)n) {
        ++r;
        cum += r;
    }

    /* previous arrays for u and v */
    size_t prev_size = 1;
    seq_t *u_prev = SEQ_ALLOC(prev_size);
    seq_t *v_prev = SEQ_ALLOC(prev_size);
    u_prev[0] = (seq_t)1;
    v_prev[0] = (seq_t)1;

    /* Generate rows up to r */
    for (size_t i = 2; i <= r; ++i) {
        size_t curr_size = i;

        seq_t *new_u = SEQ_ALLOC(curr_size);
        seq_t *new_v = SEQ_ALLOC(curr_size);

        /* u_new[0] = 1 */
        new_u[0] = (seq_t)1;

        /* u_new[j] = u_prev[j-1] + v_prev[j-1]  for j>=1 */
        for (size_t j = 1; j < curr_size; ++j) {
            new_u[j] = u_prev[j-1] + v_prev[j-1];
        }

        /* v_new[0] = u_prev[0] + v_prev[0] + 1 */
        new_v[0] = u_prev[0] + v_prev[0] + (seq_t)1;

        /* v_new[j] = v_prev[j] + u_prev[j] + u_prev[j-1]  (treat out of bounds as 0) */
        for (size_t j = 1; j < curr_size; ++j) {
            seq_t vprev_j = (j < prev_size) ? v_prev[j] : (seq_t)0;
            seq_t uprev_j = (j < prev_size) ? u_prev[j] : (seq_t)0;
            seq_t uprev_jm1 = u_prev[j-1];
            new_v[j] = vprev_j + uprev_j + uprev_jm1;
        }

        SEQ_FREE(u_prev);
        SEQ_FREE(v_prev);
        u_prev = new_u;
        v_prev = new_v;
        prev_size = curr_size;
    }

    /* compute the index within the final row */
    size_t prev_cum = cum - r;         /* total terms before row r */
    size_t offset   = (size_t)n - prev_cum;  /* 1-based within the row */
    size_t idx_in_row = offset - 1;        /* 0-based index in v_prev */

    seq_t result = v_prev[idx_in_row];

    SEQ_FREE(u_prev);
    SEQ_FREE(v_prev);
    return result;
}
