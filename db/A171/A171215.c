/*
 * A171215  Row cubed sums of triangle of Lucas polynomials (A034807) for n>0:
 *          Sum_{k=0..[n/2]} A034807(n,k)^3.
 * https://oeis.org/A171215
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = sum_{k=0..floor(n/2)} A034807(n,k)^3 * (Row cubed sums of
 *            the triangle of Lucas polynomials)
 * Range:     u8 n <= 5, u16 n <= 9, u32 n <= 17, u64 n <= 33, u128 n <= 64,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <string.h>

seq_t a(idx_t n)
{
    if (n == 0)          /* offset is 1 – this value is unused */
        return (seq_t)0;

    /* static state to keep the computed rows */
    static idx_t max_n = 0;     /* largest n that has been fully computed */
    static seq_t *sum_arr = NULL;   /* a(1..n) */
    static seq_t *prev1   = NULL;   /* row n-1  (coefficients A034807(n-1,k)) */
    static seq_t *prev2   = NULL;   /* row n-2  (coefficients A034807(n-2,k)) */
    static seq_t *cur     = NULL;   /* temporary row i   */
    static idx_t cap      = 0;      /* capacity of coefficient arrays */

    /* ---- first call or need to enlarge storage ---- */
    if (max_n == 0 || n > max_n) {
        /* size for coefficients: need indices up to floor(n/2) */
        idx_t new_cap = n / 2 + 2;
        /* reallocate sum array */
        if (max_n == 0) {
            sum_arr = (seq_t *)malloc((n + 1) * sizeof(seq_t));
        } else {
            sum_arr = (seq_t *)realloc(sum_arr, (n + 1) * sizeof(seq_t));
        }
        memset(sum_arr, 0, (n + 1) * sizeof(seq_t));

        if (max_n == 0) {
            /* allocate coefficient arrays */
            prev1 = (seq_t *)malloc(new_cap * sizeof(seq_t));
            prev2 = (seq_t *)malloc(new_cap * sizeof(seq_t));
            cur   = (seq_t *)malloc(new_cap * sizeof(seq_t));
        } else if (new_cap > cap) {
            prev1 = (seq_t *)realloc(prev1, new_cap * sizeof(seq_t));
            prev2 = (seq_t *)realloc(prev2, new_cap * sizeof(seq_t));
            cur   = (seq_t *)realloc(cur,   new_cap * sizeof(seq_t));
        }
        memset(prev1, 0, new_cap * sizeof(seq_t));
        memset(prev2, 0, new_cap * sizeof(seq_t));
        memset(cur,   0, new_cap * sizeof(seq_t));
        cap = new_cap;

        /* ------------ initialise base rows -------------- */
        /* n = 0 : L0(x) = 2   => coefficient 2 at k = 0 */
        prev2[0] = (seq_t)2;           /* will serve as row 0            */

        /* n = 1 : L1(x) = x   => coefficient 1 at k = 0 */
        prev1[0] = (seq_t)1;           /* row 1                          */
        sum_arr[1] = (seq_t)1;         /* 1^3                            */

        max_n = 1;
    }

    /* ---- extend computation to requested n ---- */
    for (idx_t i = max_n + 1; i <= n; ++i) {
        idx_t limit_i     = i / 2;
        idx_t limit_prev1 = (i - 1) / 2;
        idx_t limit_prev2 = (i - 2) / 2;

        /* compute coefficients of row i */
        cur[0] = prev1[0];            /* T(i,0) = T(i-1,0)            */

        for (idx_t k = 1; k <= limit_i; ++k) {
            seq_t v1 = 0;
            if (k <= limit_prev1) v1 = prev1[k];
            seq_t v2 = 0;
            if ((k >= 1) && (k - 1 <= limit_prev2)) v2 = prev2[k - 1];
            cur[k] = v1 + v2;
        }

        /* sum of cubes of all coefficients in this row */
        seq_t sum = (seq_t)0;
        for (idx_t k = 0; k <= limit_i; ++k) {
            seq_t v = cur[k];
            seq_t cube = v * v * v;
            sum += cube;
        }
        sum_arr[i] = sum;

        /* rotate row buffers: new prev2 = old prev1,
           new prev1 = cur, cur will hold old prev2 next round */
        seq_t *tmp = prev2;
        prev2 = prev1;
        prev1 = cur;
        cur = tmp;
    }

    max_n = n;
    return sum_arr[n];
}
