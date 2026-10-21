/*
 * A054129  T(n,3), array T as in A054126.
 * https://oeis.org/A054129
 *
 * Offset:    3    (a(3) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 8*a(n-1) - 27*a(n-2) + 50*a(n-3) - 55*a(n-4) + 36*a(n-5)
 *            - 13*a(n-6) + 2*a(n-7) + rhs(n) where rhs(3)=2, rhs(4)= -7,
 *            rhs(5)=21, rhs(6)= -35, rhs(7)=35, rhs(8)= -21, rhs(9)=7,
 *            rhs(10)= -1, and rhs(n)=0 for n>10. The first suitable term is
 *            at n=3. For n<3 the behaviour is undefined
 * Range:     u8 n <= 6, u16 n <= 13, u32 n <= 28, u64 n <= 28, u128 n <= 28,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n)
{
    if (n < 3) /* behaviour for n < 3 is irrelevant */
        return (seq_t)0;

    /* static cache of computed values up to current maximum n */
    static seq_t *cache      = NULL;
    static size_t cache_len  = 0;
    static idx_t  max_cached = 2;   /* last index initialized (0,1,2) */

    /* initial allocation: indices 0,1,2 set to 0 */
    if (!cache) {
        cache_len = 3;
        cache = (seq_t *)malloc(cache_len * sizeof(seq_t));
        cache[0] = cache[1] = cache[2] = (seq_t)0;
    }

    /* enlarge cache if necessary */
    if ((size_t)n >= cache_len) {
        size_t new_len = (size_t)n + 1;
        seq_t *tmp = (seq_t *)realloc(cache, new_len * sizeof(seq_t));
        if (!tmp) return (seq_t)0; /* allocation failure – should not happen */
        /* initialise new slots with 0 (useful for safety) */
        for (size_t i = cache_len; i < new_len; ++i) tmp[i] = (seq_t)0;
        cache = tmp;
        cache_len = new_len;
    }

    /* coefficients of rhs(n) for n = 0..10; values beyond 10 are 0 */
    const int rhs[11] = {0, 0, 0,
                         2, -7, 21, -35, 35, -21, 7, -1};

    /* compute missing values up to n */
    for (idx_t i = max_cached + 1; i <= n; ++i) {
        seq_t pos_sum = (seq_t)0;   /* contributions with positive sign */
        seq_t neg_sum = (seq_t)0;   /* contributions to be subtracted */

        /* positive terms */
        if (i >= 1) pos_sum += (seq_t)8    * cache[i - 1];
        if (i >= 3) pos_sum += (seq_t)50   * cache[i - 3];
        if (i >= 5) pos_sum += (seq_t)36   * cache[i - 5];
        if (i >= 7) pos_sum += (seq_t)2    * cache[i - 7];

        /* negative terms */
        if (i >= 2) neg_sum += (seq_t)27   * cache[i - 2];
        if (i >= 4) neg_sum += (seq_t)55   * cache[i - 4];
        if (i >= 6) neg_sum += (seq_t)13   * cache[i - 6];

        seq_t val = pos_sum - neg_sum;     /* safe: pos_sum ≥ neg_sum */

        /* add rhs contribution if any */
        if (i < 11) {
            int r = rhs[i];
            if (r > 0)
                val += (seq_t)r;
            else
                val -= (seq_t)(-r);
        }

        cache[i] = val;
    }
    max_cached = n;

    return cache[n];
}
