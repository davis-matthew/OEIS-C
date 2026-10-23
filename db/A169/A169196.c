/*
 * A169196  Number of reduced words of length n in Coxeter group on 39
 *          generators S_i with relations (S_i)^2 = (S_i S_j)^27 = I.
 * https://oeis.org/A169196
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 37*(a(n-1)+...+a(n-26)) - 703*a(n-27) for n≥27 Initial
 *            values are computed from the rational generating function
 *            a(0)=1, a(1)=39, a(2)=1482, a(3)=56316, a(4)=2140008, …
 * Range:     u8 n <= 1, u16 n <= 3, u32 n <= 6, u64 n <= 12, u128 n <= 14,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Includes needed for malloc/realloc */
#include <stdlib.h>

seq_t a(idx_t n)
{
    /* Static storage for previously computed values */
    static seq_t *val = NULL;
    static idx_t sz   = 0;          /* number of elements stored in val */

    /* Grow storage if necessary */
    if (n >= sz) {
        idx_t new_sz = n + 1;         /* we need indices 0 … n inclusive */
        seq_t *tmp = (seq_t *)realloc(val, new_sz * sizeof(seq_t));
        if (!tmp) abort();           /* allocation failed */
        val = tmp;
        /* Compute missing terms */
        for (idx_t i = sz; i <= n; ++i) {
            seq_t v;
            if (i <= 26) {
                /* i∈[0,26] : a[i] = p[i] + 37*sum_{k=1}^{i} val[i-k]   */
                seq_t sum = 0;
                for (idx_t k = 1; k <= i; ++k)
                    sum += val[i - k];
                if (i == 0)
                    v = (seq_t)1;
                else
                    v = (seq_t)2 + (seq_t)37 * sum;
            } else if (i == 27) {
                /* i = 27 : a[27] = 1 + 37*sum_{k=1}^{26} val[27-k] - 703*val[0] */
                seq_t sum = 0;
                for (idx_t k = 1; k <= 26; ++k)
                    sum += val[27 - k];
                v = (seq_t)1 + (seq_t)37 * sum - (seq_t)703 * val[0];
            } else { /* i > 27 : recurrence */
                seq_t sum = 0;
                for (idx_t k = 1; k <= 26; ++k)
                    sum += val[i - k];
                v = (seq_t)37 * sum - (seq_t)703 * val[i - 27];
            }
            val[i] = v;
        }
        sz = new_sz;
    }
    return val[n];
}
