/*
 * A049954  a(n) = a(1) + a(2) + ... + a(n-1) + a(m) for n >= 4, where m = n -
 *          1 - 2^p and p is the unique integer such that 2^p < n - 1 <=
 *          2^(p+1), starting with a(1) = 1 and a(2) = a(3) = 2.
 * https://oeis.org/A049954
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 9, u16 n <= 17, u32 n <= 32, u64 n <= 32, u128 n <= 32,
 *            big any n
 * Helpers:   prev_power_of_two
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Recurrence: a(1)=1, a(2)=a(3)=2;
   for n>=4, a(n)=a(1)+…+a(n-1)+a(m)
   where m=n-1-2^p and p is such that 2^p < n-1 <= 2^(p+1)  */

#include <stdlib.h>
#include "../utils/prev_power_of_two.h"

seq_t a(idx_t n)
{
    /* static cache of computed values */
    static seq_t *terms   = NULL;  /* 1‑based indexing */
    static seq_t *cumsum  = NULL;  /* cumulative sums a[1]+…+a[i] */
    static idx_t  size    = 0;     /* allocated capacity (>= last index) */
    static idx_t  last    = 3;     /* highest index that has been filled */

    if (n == 0)
        return (seq_t)0;  /* undefined, but harmless */

    if (terms == NULL) {
        /* initialise with capacity for at least 4 entries */
        size = (n > 4) ? n : 4;
        terms  = (seq_t *)malloc((size + 1) * sizeof(seq_t));
        cumsum = (seq_t *)malloc((size + 1) * sizeof(seq_t));
        if (!terms || !cumsum)
            return (seq_t)0;  /* out of memory */

        terms[1] = (seq_t)1;
        terms[2] = (seq_t)2;
        terms[3] = (seq_t)2;

        cumsum[1] = (seq_t)1;
        cumsum[2] = (seq_t)3;      /* 1+2 */
        cumsum[3] = (seq_t)5;      /* 1+2+2 */
    } else if (n > size) {
        /* grow to next power of two that fits n */
        idx_t newsize = size;
        while (newsize < n)
            newsize <<= 1;
        seq_t *t = (seq_t *)realloc(terms,  (newsize + 1) * sizeof(seq_t));
        seq_t *c = (seq_t *)realloc(cumsum, (newsize + 1) * sizeof(seq_t));
        if (!t || !c)
            return (seq_t)0;  /* out of memory */
        terms  = t;
        cumsum = c;
        size   = newsize;
    }

    if (n <= last)
        return terms[n];

    /* compute terms up to n */
    for (idx_t i = last + 1; i <= n; ++i) {
        idx_t power = prev_power_of_two(i - 1);    /* 2^p < i-1 <= 2^(p+1)  */
        idx_t m = (i - 1) - power;                 /* extra index */

        seq_t sum_prev = cumsum[i - 1];
        seq_t am = terms[m];
        terms[i]   = sum_prev + am;
        cumsum[i]  = sum_prev + terms[i];
    }
    last = n;
    return terms[n];
}
