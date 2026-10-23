/*
 * A192547  sum(stirling1(n,k)*stirling1(n+1,k+1),k=0..n).
 * https://oeis.org/A192547
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Σ_{k=0..n} stirling1(n,k) * stirling1(n+1,k+1)
 * Range:     u8 n <= 3, u16 n <= 5, u32 n <= 8, u64 n <= 12, u128 n <= 20,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n)
{
    /* Static data persistent between calls */
    static seq_t *curr = NULL;            /* s(current_n, *) */
    static idx_t curr_len = 0;             /* length of curr array   */
    static idx_t current_n = 0;            /* n value for curr       */
    static seq_t *a_vals = NULL;          /* a values computed so far */
    static size_t a_cap = 0;               /* capacity of a_vals     */
    static idx_t a_cnt = 0;                /* number of a values stored */

    /* Initialisation: first row s(0,*) */
    if (!curr) {
        curr_len = 1;
        curr = (seq_t *)malloc(sizeof(seq_t) * curr_len);
        if (!curr) return 0;             /* Should not happen, but guard against OOM */
        curr[0] = (seq_t)1;              /* s(0,0)=1 */
        current_n = 0;
    }

    /* Grow a_vals to be able to store up to n */
    while (a_cnt <= n) {
        /* Ensure a_vals has enough space for a_cnt+1 elements */
        if (a_cnt >= a_cap) {
            size_t new_cap = (a_cap == 0) ? 1 : a_cap * 2;
            seq_t *tmp = (seq_t *)realloc(a_vals, new_cap * sizeof(seq_t));
            if (!tmp) return 0;           /* OOM guard */
            a_vals = tmp;
            a_cap = new_cap;
        }

        /* Compute the next Stirling row (s(current_n + 1, *)) */
        idx_t next_len = curr_len + 1;
        seq_t *next = (seq_t *)malloc(sizeof(seq_t) * next_len);
        if (!next) return 0;               /* OOM guard */

        next[0] = (seq_t)0;                 /* s(*,0)=0 by definition */
        for (idx_t j = 1; j <= curr_len; ++j) {
            seq_t left  = curr[j - 1];
            seq_t right = (j < curr_len) ? curr[j] : (seq_t)0;
            next[j] = left - (seq_t)current_n * right;
        }

        /* Evaluate the sum defining a(current_n) */
        seq_t sum = (seq_t)0;
        for (idx_t k = 0; k < curr_len; ++k) {
            sum += curr[k] * next[k + 1];
        }

        /* Store the result */
        a_vals[a_cnt] = sum;

        /* Prepare for next iteration */
        free(curr);
        curr = next;
        curr_len = next_len;
        current_n++;                /* curr now represents s(current_n, *)   */
        a_cnt++;                    /* we have stored one more a value      */
    }

    return a_vals[n];
}
