/*
 * A170233  Number of reduced words of length n in Coxeter group on 32
 *          generators S_i with relations (S_i)^2 = (S_i S_j)^40 = I.
 * https://oeis.org/A170233
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(0) = 1 a(n) = 32 * 31^(n-1) for n >= 1 (Number of words of
 *            length n over 32 symbols with no adjacent repeated symbols)
 * Range:     u8 n <= 1, u16 n <= 3, u32 n <= 6, u64 n <= 12, u128 n <= 15,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n)
{
    /* Handle the offset */
    if (n == 0)
        return (seq_t)1;

    /* Convert n to standard type for indexing */
    idx_t idx = n - 1;          /* exponent of 31 */
    static seq_t *pow31 = NULL;
    static size_t len   = 0;    /* number of stored powers (len = highest index + 1) */
    static size_t cap   = 0;    /* capacity of allocated array */

    /* Initialize array if necessary */
    if (pow31 == NULL) {
        cap = 8;                  /* initial capacity */
        pow31 = (seq_t *)malloc(sizeof(seq_t) * cap);
        if (!pow31) return (seq_t)0;  /* allocation failure */
        pow31[0] = (seq_t)1;          /* 31^0 */
        len = 1;
    }

    /* Extend array to accommodate up to idx */
    if (len <= idx) {
        size_t newcap = cap;
        if (newcap == 0) newcap = 1;
        while (newcap <= idx) newcap <<= 1;   /* double until enough */

        seq_t *tmp = (seq_t *)realloc(pow31, sizeof(seq_t) * newcap);
        if (!tmp) return (seq_t)0;            /* allocation failure */
        pow31 = tmp;
        cap = newcap;

        /* Compute new powers */
        for (size_t i = len; i <= idx; ++i)
            pow31[i] = pow31[i - 1] * (seq_t)31;
        len = idx + 1;
    }

    return (seq_t)32 * pow31[idx];
}
