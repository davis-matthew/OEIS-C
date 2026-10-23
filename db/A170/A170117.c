/*
 * A170117  Number of reduced words of length n in Coxeter group on 12
 *          generators S_i with relations (S_i)^2 = (S_i S_j)^38 = I.
 * https://oeis.org/A170117
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 2, u16 n <= 4, u32 n <= 9, u64 n <= 17, u128 n <= 17,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Recurrence: a(0)=1;
   for 1<=n<=37:   a(n)=2+10*sum_{i=0}^{n-1}a(i);
   for n=38:       a(38)=1+10*sum_{i=1}^{37}a(i)-55*a(0);
   for n>=39:      a(n)=10*sum_{i=n-37}^{n-1}a(i)-55*a(n-38). */

#include <stdlib.h>

seq_t a(idx_t n)
{
    static seq_t *a_cache = NULL;
    static seq_t *prefix = NULL;
    static idx_t cache_len = 0;

    /* initialize for n=0 */
    if (cache_len == 0) {
        size_t init_cap = 2;
        a_cache = (seq_t *)malloc(init_cap * sizeof(seq_t));
        prefix   = (seq_t *)malloc(init_cap * sizeof(seq_t));
        if (!a_cache || !prefix) { while(1); }              /* unrecoverable error */
        a_cache[0] = (seq_t)1;
        prefix[0]  = (seq_t)1;
        cache_len  = 1;
    }

    /* expand cache if needed */
    if (n >= cache_len) {
        idx_t needed = n + 1;
        size_t cap = cache_len ? cache_len : 2;
        while (cap <= n) cap <<= 1;

        a_cache = (seq_t *)realloc(a_cache, cap * sizeof(seq_t));
        prefix  = (seq_t *)realloc(prefix,  cap * sizeof(seq_t));
        if (!a_cache || !prefix) { while(1); }              /* unrecoverable error */

        /* compute new entries */
        for (idx_t i = cache_len; i <= n; ++i) {
            if (i == 0) continue;           /* already set */

            if (i <= 37) {
                seq_t sum = prefix[i - 1];
                a_cache[i] = (seq_t)2 + (seq_t)10 * sum;
                prefix[i]  = prefix[i - 1] + a_cache[i];
            } else if (i == 38) {
                seq_t sum1to37 = prefix[37] - prefix[0];
                a_cache[38] = (seq_t)1 + (seq_t)10 * sum1to37 - (seq_t)55 * a_cache[0];
                prefix[38]  = prefix[37] + a_cache[38];
            } else { /* i >= 39 */
                seq_t sum37 = prefix[i - 1] - prefix[i - 38];
                a_cache[i] = (seq_t)10 * sum37 - (seq_t)55 * a_cache[i - 38];
                prefix[i]  = prefix[i - 1] + a_cache[i];
            }
        }
        cache_len = n + 1;
    }

    return a_cache[n];
}
