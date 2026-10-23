/*
 * A132304  Sum of fourth powers of trinomial coefficients: a(n) =
 *          Sum_{k=0..2n} trinomial(n,k)^4 where trinomial(n,k) = [x^k] (1 + x
 *          + x^2)^n.
 * https://oeis.org/A132304
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Sum_{k=0..2n} trinomial(n,k)^4 where trinomial(n,k) =
 *            [x^k] (1 + x + x^2)^n (OEIS A132304)
 * Range:     u8 n <= 2, u16 n <= 3, u32 n <= 6, u64 n <= 11, u128 n <= 16,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n) {
    /* static state for memoisation and dynamic programming   */
    static idx_t max_n = 0;          /* largest index computed so far    */
    static seq_t *a_vals = NULL;     /* a_vals[0..max_n]                 */
    static seq_t *prev_row = NULL;   /* coefficients for n-1             */
    static seq_t *cur_row  = NULL;   /* coefficients for current n       */
    static idx_t prev_len = 0;       /* length of prev_row (2*(m-1)+1)   */

    /* basic case: a(0) = 1 */
    if (n == 0) {
        if (a_vals == NULL) {
            a_vals = (seq_t *)malloc(sizeof(seq_t));
            a_vals[0] = (seq_t)1;
        }
        return a_vals[0];
    }

    /* ensure the a_vals array can hold up to n                */
    if (a_vals == NULL || max_n < n) {
        seq_t *tmp = (seq_t *)realloc(a_vals, (n + 1) * sizeof(seq_t));
        a_vals = tmp;
    }

    /* return cached value if already computed */
    if (max_n >= n)
        return a_vals[n];

    /* initialise the first row (n = 0) if necessary */
    if (prev_row == NULL) {
        prev_row = (seq_t *)malloc(sizeof(seq_t));  /* one element */
        prev_row[0] = (seq_t)1;                    /* (1 + x + x^2)^0 = 1 */
        prev_len = 1;
    }

    /* build successive rows from max_n+1 up to n */
    for (idx_t m = max_n + 1; m <= n; ++m) {
        idx_t cur_len = 2 * m + 1;          /* length of row m */

        /* resize cur_row to the needed length */
        cur_row = (seq_t *)realloc(cur_row, cur_len * sizeof(seq_t));

        /* compute the m‑th row from the (m‑1)‑th row   */
        for (idx_t k = 0; k < cur_len; ++k) {
            seq_t val = 0;
            if (k < prev_len)                 val += prev_row[k];
            if (k > 0 && k - 1 < prev_len)    val += prev_row[k - 1];
            if (k > 1 && k - 2 < prev_len)    val += prev_row[k - 2];
            cur_row[k] = val;
        }

        /* compute a(m) = sum of fourth powers of the row   */
        seq_t sum = (seq_t)0;
        for (idx_t k = 0; k < cur_len; ++k) {
            seq_t pk = cur_row[k];
            /* pk^4: pk * pk * pk * pk */
            pk = pk * pk;            /* pk^2          */
            pk = pk * cur_row[k];    /* pk^3          */
            pk = pk * cur_row[k];    /* pk^4          */
            sum += pk;
        }
        a_vals[m] = sum;

        /* prepare for next iteration: previous row becomes current   */
        seq_t *tmp = prev_row;
        prev_row = cur_row;
        cur_row = tmp;
        prev_len = cur_len;
    }

    max_n = n;
    return a_vals[n];
}
