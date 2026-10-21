/*
 * A224149  Number of 5 X n 0..1 arrays with rows and antidiagonals unimodal
 *          and columns nondecreasing.
 * https://oeis.org/A224149
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 11*a(n-1) - 55*a(n-2) + 165*a(n-3) - 330*a(n-4) +
 *            462*a(n-5) - 462*a(n-6) + 330*a(n-7) * - 165*a(n-8) + 55*a(n-9)
 *            - 11*a(n-10) + a(n-11) for n > 11. * * Initial values (offset
 *            1): a(1)=6, a(2)=36, a(3)=155, a(4)=526, a(5)=1509, * a(6)=3827,
 *            a(7)=8838, a(8)=18969, a(9)=38392, a(10)=74053, a...
 * Range:     u8 n <= 3, u16 n <= 9, u32 n <= 39, u64 n <= 210, u128 n <= 210,
 *            big any n
 * Helpers:   ensure_cache
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include "../utils/ensure_cache.h"

static seq_t *a_cache = NULL;           /* cache for sequence values */
static size_t cache_cap = 0;             /* allocated capacity (>= n+1) */
static idx_t  cached_n  = 0;             /* highest n computed and stored */

seq_t a(idx_t n)
{
    if (n <= cached_n)
        return a_cache[n];

    /* Ensure the cache can hold indices up to n (index 0 unused). */
    ensure_cache(n, &a_cache, &cache_cap);

    for (idx_t k = cached_n + 1; k <= n; ++k)
    {
        if (k == 1)          a_cache[k] = 6;
        else if (k == 2)      a_cache[k] = 36;
        else if (k == 3)      a_cache[k] = 155;
        else if (k == 4)      a_cache[k] = 526;
        else if (k == 5)      a_cache[k] = 1509;
        else if (k == 6)      a_cache[k] = 3827;
        else if (k == 7)      a_cache[k] = 8838;
        else if (k == 8)      a_cache[k] = 18969;
        else if (k == 9)      a_cache[k] = 38392;
        else if (k == 10)     a_cache[k] = 74053;
        else if (k == 11)     a_cache[k] = 137204;
        else
        {
            /* Use the recurrence with only non‑negative intermediate results. */
            seq_t val = a_cache[k - 11];
            seq_t tmp;

            tmp = a_cache[k - 1] - a_cache[k - 10];
            val += tmp * (seq_t)11;

            tmp = a_cache[k - 9] - a_cache[k - 2];
            val += tmp * (seq_t)55;

            tmp = a_cache[k - 3] - a_cache[k - 8];
            val += tmp * (seq_t)165;

            tmp = a_cache[k - 5] - a_cache[k - 6];
            val += tmp * (seq_t)462;

            /* The last term is subtracted; the difference is positive. */
            tmp = a_cache[k - 4] - a_cache[k - 7];
            val -= tmp * (seq_t)330;

            a_cache[k] = val;
        }
    }
    cached_n = n;
    return a_cache[n];
}
