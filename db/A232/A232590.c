/*
 * A232590  Number of (1+1)X(n+1) 0..2 arrays with every element next to
 *          itself plus and minus one within the range 0..2 horizontally or
 *          antidiagonally, with no adjacent elements equal.
 * https://oeis.org/A232590
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) Empirical recurrence (valid for n ≥ 6): a(n) = -a(n-1) +
 *            2*a(n-2) + 4*a(n-3) + 3*a(n-4) + a(n-5) with initial values:
 *            a(1)=0, a(2)=10, a(3)=2, a(4)=26, a(5)=20 This implementation
 *            uses memoisation and the helper ensure_cache(idx_t n, seq_t
 *            **cache_ptr, size_t *cap_ptr) to grow the temporary...
 * Range:     u8 n <= 8, u16 n <= 17, u32 n <= 35, u64 n <= 72, u128 n <= 144,
 *            big any n
 * Helpers:   ensure_cache
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>                     /* for size_t */
#include "../utils/ensure_cache.h"     /* provides ensure_cache function */

seq_t a(idx_t n)
{
    if (n < 1) return 0;               /* out-of-range guard, though offset = 1 */

    /* Static cache to keep previously computed terms across calls */
    static seq_t *cache = NULL;
    static size_t cache_cap = 0;
    static idx_t cache_max = 0;        /* maximum index already computed */

    /* Grow cache to accommodate up to n */
    if (n > cache_max)
    {
        /* ensure_cache guarantees storage for indices 0..n (we use 1-based) */
        ensure_cache(n, &cache, &cache_cap);

        /* Fill new entries */
        for (idx_t i = cache_max + 1; i <= n; ++i)
        {
            seq_t val;
            switch (i)
            {
                case 1: val = 0;   break;
                case 2: val = 10;  break;
                case 3: val = 2;   break;
                case 4: val = 26;  break;
                case 5: val = 20;  break;
                default:
                    /* recurrence: a(i) = 2*a(i-2) + 4*a(i-3) + 3*a(i-4) + a(i-5) - a(i-1) */
                    val = cache[i-2] * (seq_t)2;
                    val += cache[i-3] * (seq_t)4;
                    val += cache[i-4] * (seq_t)3;
                    val += cache[i-5];
                    /* Safe subtraction because recurrence guarantees non‑negative result */
                    val -= cache[i-1];
                    break;
            }
            cache[i] = val;
        }
        cache_max = n;
    }

    return cache[n];
}
