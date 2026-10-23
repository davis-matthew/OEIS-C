/*
 * A143006  a(0) = 0, a(1) = 1, a(n+1) = (2*n+1)*(n^2+n+41)*a(n) - n^6*a(n-1).
 * https://oeis.org/A143006
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(0) = 0, a(1) = 1, a(n+1) = (2*n+1)*(n^2+n+41)*a(n) -
 *            n^6*a(n-1)
 * Range:     u8 n <= 2, u16 n <= 3, u32 n <= 4, u64 n <= 8, u128 n <= 13,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n)
{
    /* static cache of computed values */
    static seq_t *cache = NULL;
    static idx_t cache_cap = 0;
    static idx_t last_filled = 0;  /* highest index computed so far */

    /* ensure capacity for at least the requested index */
    if (n >= cache_cap) {
        idx_t new_cap = (n + 1);
        if (cache_cap == 0) {
            new_cap = 2; /* minimal capacity for base cases */
        } else {
            /* grow roughly twice as much to keep amortized linear time */
            if (new_cap < cache_cap * 2)
                new_cap = cache_cap * 2;
        }
        cache = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
        cache_cap = new_cap;
    }

    /* initialize base cases if not already done */
    if (last_filled == 0) {
        cache[0] = (seq_t)0;
        last_filled = 0;
    }
    if (n >= 1 && last_filled < 1) {
        cache[1] = (seq_t)1;
        last_filled = 1;
    }

    /* compute any missing terms up to n */
    for (idx_t i = last_filled + 1; i <= n; ++i) {
        idx_t i_minus = i - 1;                     /* n in the recurrence */
        seq_t ip = (seq_t)i_minus;                 /* (i-1) as seq_t */

        /* factor (2*n+1) => 2*(i-1)+1 = 2*i-1 */
        seq_t two_i_minus1 = (seq_t)(2 * i - 1);

        /* (n^2 + n + 41) */
        seq_t ip_sq = ip * ip;                     /* (i-1)^2 */
        seq_t factor1 = ip_sq + ip + (seq_t)41;    /* n^2 + n + 41 */

        /* first term: (2*n+1)*(n^2+n+41)*a(n) */
        seq_t prod1 = two_i_minus1 * factor1 * cache[i_minus];

        /* n^6 = (i-1)^6 = (i-1)^4 * (i-1)^2 */
        seq_t pow4 = ip_sq * ip_sq;                /* (i-1)^4 */
        seq_t pow6 = pow4 * ip_sq;                 /* (i-1)^6 */

        /* second term: n^6 * a(n-1) */
        seq_t prod2 = pow6 * cache[i_minus - 1];

        cache[i] = prod1 - prod2;
    }
    last_filled = n;
    return cache[n];
}
