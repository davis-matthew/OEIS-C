/*
 * A209693  Triangle of coefficients of polynomials u(n,x) jointly generated
 *          with A209694; see the Formula section.
 * https://oeis.org/A209693
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, tabl
 * Formula:   Triangle of coefficients of polynomials u(n,x) jointly generated
 *            * with A209694. An online algorithm with memoisation is used;
 *            the * sequence a(n) is the flattened array of the coefficients
 *            of u(n,x) * where u(n,x) has degree n‑1. The recurrence for the
 *            polynomials is * * u(n,x) = x*u(n‑1,x) + x*...
 * Range:     u8 n <= 32, u16 n <= 65, u32 n <= 65, u64 n <= 65, u128 n <= 65,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n)
{
    /* Static state for memoisation */
    static idx_t    max_row     = 0;         /* maximum row that has been computed */
    static idx_t    cur_size    = 0;         /* current degree + 1 of u/v arrays */
    static seq_t  *u_cur       = NULL;       /* current u coefficients   */
    static seq_t  *v_cur       = NULL;       /* current v coefficients   */
    static seq_t  *seq_cache   = NULL;       /* flattened sequence values*/
    static size_t  seq_cap     = 0;          /* capacity of seq_cache    */
    static idx_t   seq_used    = 0;          /* number of values stored */

    if (n < 1) return 0;                     /* undefined for n<1 */

    /*--- initialise on first call ------------------------------------*/
    if (max_row == 0) {
        /*  First row (n = 1) */
        seq_cap   = 1;
        seq_cache = (seq_t *)malloc(seq_cap * sizeof(seq_t));
        u_cur     = (seq_t *)calloc(1, sizeof(seq_t));
        v_cur     = (seq_t *)calloc(1, sizeof(seq_t));
        u_cur[0]  = 1;          /* u(1,x)=1  */
        v_cur[0]  = 1;          /* v(1,x)=1  */
        seq_cache[seq_used++] = u_cur[0];  /* a(1)=1   */
        cur_size = 1;
        max_row  = 1;
    }

    /*--- compute rows until we have at least n terms -----------------*/
    while (seq_used < n) {
        idx_t new_len = cur_size + 1;        /* degree+1 for next row */

        /* allocate arrays for next step */
        seq_t *u_next = (seq_t *)calloc(new_len, sizeof(seq_t));
        seq_t *v_next = (seq_t *)calloc(new_len, sizeof(seq_t));

        /* u_next[0] = 0,   u_next[i] = u_cur[i-1] + v_cur[i-1]  (i>=1) */
        u_next[0] = 0;
        for (idx_t i = 1; i < new_len; ++i)
            u_next[i] = u_cur[i-1] + v_cur[i-1];

        /* v_next[0] = u_cur[0] + v_cur[0] + 1
           v_next[i] = u_cur[i] + v_cur[i] + v_cur[i-1] (1 <= i < new_len-1)
           v_next[new_len-1] = v_cur[new_len-2]  */
        v_next[0] = u_cur[0] + v_cur[0] + (seq_t)1;
        for (idx_t i = 1; i < new_len-1; ++i)
            v_next[i] = u_cur[i] + v_cur[i] + v_cur[i-1];
        v_next[new_len-1] = v_cur[new_len-2];

        /* free old arrays and rotate */
        free(u_cur);
        free(v_cur);
        u_cur = u_next;
        v_cur = v_next;
        cur_size = new_len;

        /*--- append new row's u coefficients to the sequence cache ---*/
        if (seq_used + new_len > seq_cap) {
            size_t new_cap = seq_cap * 2 + new_len;
            if (new_cap < seq_used + new_len) new_cap = seq_used + new_len;
            seq_t *tmp = (seq_t *)realloc(seq_cache, new_cap * sizeof(seq_t));
            seq_cache = tmp;
            seq_cap   = new_cap;
        }
        for (idx_t i = 0; i < new_len; ++i)
            seq_cache[seq_used++] = u_cur[i];

        ++max_row;    /* we have now computed another row */
    }

    return seq_cache[n-1];
}
