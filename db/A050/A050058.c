/*
 * A050058  a(n) = a(n-1)+a(m), where m=n-1-2^p and 2^p<n-1<=2^(p+1), for n >=
 *          4.
 * https://oeis.org/A050058
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(1)=1, a(2)=3, a(3)=1; for n≥4 a(n) = a(n-1)+a(m) where m =
 *            n-1-2^p and 2^p < n-1 ≤ 2^(p+1) (i.e. p is the position of the
 *            highest set bit of n-1). The sequence is memoised so calls to
 *            a() can be made in any order
 * Range:     u8 n <= 32, u16 n <= 201, u32 n <= 1764, u64 n <= 8193,
 *            u128 n <= 8193, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>  /* malloc, realloc, free */

seq_t a(idx_t n)
{
    /* Cache storage (index 0 corresponds to a(1)) */
    static seq_t *cache   = NULL;
    static idx_t  cached  = 0;   /* largest n already computed */
    static idx_t  capacity = 0;  /* current capacity of cache */

    if (n == 0)      /* out‑of‑range, but harmless; offset is 1 */
        return (seq_t)0;

    /* Return memoised value if already known */
    if (n <= cached)
        return cache[n-1];

    /* Grow cache to hold n elements */
    if (n > capacity) {
        idx_t newcap = n;
        seq_t *tmp = (seq_t *)realloc(cache, newcap * sizeof(seq_t));
        if (!tmp) {          /* allocation failed – abort */
            /* In an OEIS judge, such a failure is unlikely;
               if it does happen we return 0 as a safe fallback. */
            return (seq_t)0;
        }
        cache = tmp;
        capacity = newcap;
    }

    /* Compute terms up to n */
    for (idx_t i = cached + 1; i <= n; ++i) {
        if (i == 1) {
            cache[0] = (seq_t)1;
        } else if (i == 2) {
            cache[1] = (seq_t)3;
        } else if (i == 3) {
            cache[2] = (seq_t)1;
        } else {
            /* Find p so that 2^p < i-1 <= 2^(p+1) */
            idx_t x = i - 1;
            idx_t power = 1;
            while ((power << 1) < x)
                power <<= 1;
            idx_t m = x - power;   /* 1 <= m < i-1 */
            cache[i-1] = cache[i-2] + cache[m-1];
        }
    }

    cached = n;
    return cache[n-1];
}
