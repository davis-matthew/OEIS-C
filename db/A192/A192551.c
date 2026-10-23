/*
 * A192551  a(n) = sum(abs(stirling1(n+1,k+1))*k!^2,k=0..n).
 * https://oeis.org/A192551
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Σ_{k=0..n} |s(n+1,k+1)| · (k!)² where s is the Stirling
 *            number of the first kind (absolute value)
 * Range:     u8 n <= 3, u16 n <= 5, u32 n <= 8, u64 n <= 12, u128 n <= 20,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n)
{
    /* static rows of Stirling numbers (unsigned) */
    static seq_t *st_row = NULL;     /* current row for n */
    static size_t st_len = 0;        /* number of entries in st_row ( = n+1 ) */

    /* static factorials 0! … n! */
    static seq_t *fact = NULL;
    static size_t fact_len = 0;      /* number of entries in fact ( = max index+1 ) */

    /*--- initialise base cases ------------------------------------------*/
    if (st_row == NULL) {
        st_row = (seq_t *)malloc(1 * sizeof(seq_t));   /* row for n = 0 */
        st_row[0] = 1;                                 /* s(0,0) = 1 */
        st_len = 1;
    }
    if (fact == NULL) {
        fact = (seq_t *)malloc(1 * sizeof(seq_t));   /* 0! */
        fact[0] = 1;
        fact_len = 1;
    }

    /*--- extend factorial array up to n ---------------------------------*/
    if (fact_len <= n) {
        size_t new_fact_len = n + 1;
        seq_t *tmp = (seq_t *)realloc(fact, new_fact_len * sizeof(seq_t));
        if (!tmp) return (seq_t)0;   /* allocation failure → return 0 */
        fact = tmp;
        for (idx_t i = fact_len; i <= n; ++i) {
            fact[i] = fact[i - 1] * (seq_t)i;
        }
        fact_len = new_fact_len;
    }

    /*--- extend Stirling row up to n+1 -----------------------------------*/
    idx_t cur_n = (idx_t)(st_len - 1);   /* current n for which st_row is valid */
    while (cur_n < n + 1) {
        /* compute row for cur_n + 1  (size cur_n + 2) */
        size_t new_len = cur_n + 2;
        seq_t *new_row = (seq_t *)malloc(new_len * sizeof(seq_t));
        if (!new_row) return (seq_t)0;   /* allocation failure */

        new_row[0] = 0;                     /* s(n+1,0) = 0 for n+1 > 0 */
        for (idx_t k = 1; k <= cur_n; ++k) {
            new_row[k] = st_row[k - 1] + (seq_t)cur_n * st_row[k];
        }
        new_row[cur_n + 1] = 1;              /* s(n+1,n+1) = 1 */

        free(st_row);
        st_row = new_row;
        st_len = new_len;
        ++cur_n;
    }

    /*--- compute the sum for a(n) ---------------------------------------*/
    seq_t sum = 0;
    for (idx_t k = 0; k <= n; ++k) {
        seq_t term = st_row[k + 1];      /* |s(n+1,k+1)| */
        term = term * fact[k];
        term = term * fact[k];
        sum += term;
    }
    return sum;
}
