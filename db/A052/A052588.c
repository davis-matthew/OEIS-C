/*
 * A052588  Expansion of e.g.f.: (1-x)/(1-x-x^2-x^3+x^4).
 * https://oeis.org/A052588
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, nonn
 * Formula:   not stated
 * Range:     u8 n <= 4, u16 n <= 7, u32 n <= 10, u64 n <= 17, u128 n <= 18,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* 
   OEIS A052588: a(0)=1, a(1)=0, a(2)=2, a(3)=12.
   For n≥0: a(n+4) = -P*a(n) + Q*a(n+1) + R*a(n+2) + S*a(n+3),
   where 
       P = n^4+10*n^3+35*n^2+50*n+24,
       Q = n^3+9*n^2+26*n+24,
       R = n^2+7*n+12,
       S = n+4.
*/

#include <stdlib.h>

seq_t a(idx_t n)
{
    static seq_t *cache = NULL;
    static size_t cap = 0;     /* allocated capacity */
    static size_t sz  = 0;     /* number of initialized entries */

    /* grow the cache if necessary */
    if (n >= cap) {
        size_t new_cap = cap > 0 ? cap : 1;
        while (new_cap <= n) new_cap <<= 1;
        cache = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
        cap   = new_cap;
    }

    /* compute missing values in order */
    for (idx_t i = sz; i <= n; ++i) {
        if (i == 0)          cache[0] = 1;
        else if (i == 1)     cache[1] = 0;
        else if (i == 2)     cache[2] = 2;
        else if (i == 3)     cache[3] = 12;
        else {
            idx_t m = i - 4;
            seq_t x0 = (seq_t)m;
            seq_t x2 = x0 * x0;
            seq_t x3 = x2 * x0;
            seq_t x4 = x2 * x2;

            seq_t P = x4 + (seq_t)10 * x3 + (seq_t)35 * x2
                    + (seq_t)50 * x0 + (seq_t)24;
            seq_t Q = x3 + (seq_t)9 * x2 + (seq_t)26 * x0
                    + (seq_t)24;
            seq_t R = x2 + (seq_t)7 * x0 + (seq_t)12;
            seq_t S = x0 + (seq_t)4;

            seq_t t0 = P * cache[i-4];
            seq_t t1 = Q * cache[i-3];
            seq_t t2 = R * cache[i-2];
            seq_t t3 = S * cache[i-1];

            cache[i] = t1 + t2 + t3 - t0;
        }
    }
    sz = n + 1;
    return cache[n];
}
