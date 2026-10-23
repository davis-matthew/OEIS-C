/*
 * A103487  a(0)=7; a(1)=11; a(2)=13; a(3)=17; a(n) = |a(n-2)+a(n-3)-a(n-4)|.
 * https://oeis.org/A103487
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = |a(n-2)+a(n-3)-a(n-4)|, a(0)=7,a(1)=11,a(2)=13,a(3)=17
 * Range:     u8 n <= 46, u16 n <= 63, u32 n <= 63, u64 n <= 63, u128 n <= 63,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n)
{
    /* static cache of terms already computed */
    static seq_t *term = NULL;   /* array of terms */
    static idx_t  last  = 0;     /* number of terms stored (length of array) */

    /* initialise cache with the first four terms */
    if (last == 0) {
        last = 4;
        term = (seq_t *)malloc(last * sizeof(seq_t));
        if (!term) return 0; /* allocation failure – return 0 as fallback */
        term[0] = 7;  /* a(0) */
        term[1] = 11; /* a(1) */
        term[2] = 13; /* a(2) */
        term[3] = 17; /* a(3) */
    }

    /* already computed? */
    if (n < last)
        return term[n];

    /* extend cache up to index n */
    for (idx_t i = last; i <= n; ++i) {
        seq_t a_nm2 = term[i-2];
        seq_t a_nm3 = term[i-3];
        seq_t a_nm4 = term[i-4];
        seq_t sum   = a_nm2 + a_nm3;
        seq_t val;

        /* unsigned comparison to avoid underflow */
        if (sum >= a_nm4)
            val = sum - a_nm4;
        else
            val = a_nm4 - sum;

        /* grow the array by one element */
        seq_t *tmp = (seq_t *)realloc(term, (i + 1) * sizeof(seq_t));
        if (!tmp) return 0; /* allocation failure – return 0 */
        term = tmp;
        term[i] = val;
    }

    last = n + 1;
    return term[n];
}
