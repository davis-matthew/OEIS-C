/*
 * A235082  Number of (n+1) X (3+1) 0..4 arrays with every 2 X 2 subblock
 *          having its diagonal sum differing from its antidiagonal sum by 4,
 *          with no adjacent elements equal (constant-stress tilted 1 X 1
 *          tilings).
 * https://oeis.org/A235082
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 1, u16 n <= 8, u32 n <= 22, u64 n <= 48, u128 n <= 98,
 *            big any n
 * Helpers:   seq_realloc
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Recurrence: a(n) = 5*a(n-1)+9*a(n-2)-78*a(n-3)+12*a(n-4)+462*a(n-5)
 *            -368*a(n-6)-1304*a(n-7)+1433*a(n-8)+1841*a(n-9)
 *            -2237*a(n-10)-1346*a(n-11)+1420*a(n-12)+604*a(n-13)
 *            -300*a(n-14)-120*a(n-15)   for n>15
 * Initial values: a(1)=202, a(2)=418, a(3)=830, a(4)=1758, a(5)=3610,
 * a(6)=7810, a(7)=16390, a(8)=36014, a(9)=76770, a(10)=170682,
 * a(11)=368326, a(12)=826742, a(13)=1802794, a(14)=4080058,
 * a(15)=8981550 */
 
#include <stddef.h>
#include "../utils/seq_realloc.h"
 
seq_t a(idx_t n)
{
    if (n == 0)
        return 0;   /* not used, but safe */
 
    /* static cache of computed terms, 1‑based index */
    static seq_t *cache   = NULL;
    static size_t  cap    = 0;
    static idx_t   last   = 0;
 
    /* first 15 values, stored as literals */
    static const seq_t init_vals[15] = {
        202ULL, 418ULL, 830ULL, 1758ULL, 3610ULL,
        7810ULL, 16390ULL, 36014ULL, 76770ULL, 170682ULL,
        368326ULL, 826742ULL, 1802794ULL, 4080058ULL, 8981550ULL
    };
 
    /* grow the cache if necessary */
    if (cap < (size_t)(n + 1))
    {
        size_t new_cap = (size_t)(n + 1);
        cache = seq_realloc(cache, cap, new_cap);
        cap   = new_cap;
    }
 
    /* compute values up to n */
    for (idx_t i = last + 1; i <= n; ++i)
    {
        if (i <= 15)
        {
            cache[i] = init_vals[i - 1];
        }
        else
        {
            /* sum of positive contributions */
            seq_t pos = (seq_t)5   * cache[i - 1] +
                        (seq_t)9   * cache[i - 2] +
                        (seq_t)12  * cache[i - 4] +
                        (seq_t)462 * cache[i - 5] +
                        (seq_t)1433* cache[i - 8] +
                        (seq_t)1841* cache[i - 9] +
                        (seq_t)1420* cache[i -12] +
                        (seq_t)604 * cache[i -13];
 
            /* sum of negative contributions */
            seq_t neg = (seq_t)78   * cache[i - 3] +
                        (seq_t)368  * cache[i - 6] +
                        (seq_t)1304 * cache[i - 7] +
                        (seq_t)2237 * cache[i -10] +
                        (seq_t)1346 * cache[i -11] +
                        (seq_t)300  * cache[i -14] +
                        (seq_t)120  * cache[i -15];
 
            cache[i] = pos - neg;
        }
    }
    last = n;
    return cache[n];
}
