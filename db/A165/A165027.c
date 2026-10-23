/*
 * A165027  Number of n-digit fixed points under the base-4 Kaprekar map
 *          A165012.
 * https://oeis.org/A165027
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  base, nonn
 * Formula:   a(n) = 2*a(n-2) + a(n-3) - a(n-4) - 2*a(n-5) + a(n-7) (n > 8)
 *            Initial terms (offset 1): a(1)=1, a(2)=0, a(3)=1, a(4)=1,
 *            a(5)=0, a(6)=3, a(7)=1, a(8)=3
 * Range:     u8 n <= 77, u16 n <= 200, u32 n <= 200, u64 n <= 200,
 *            u128 n <= 200, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n)
{
    /* cache holds a(i+1) at cache[i]  (0‑based) */
    static seq_t *cache = NULL;
    static idx_t cached = 0;          /* number of values cached */

    if (n == 0) return 0;             /* offset is 1, values below 1 are undefined */

    if (n <= cached) return cache[n-1];

    /* enlarge cache to hold up to n */
    size_t newsize = (size_t)n;
    cache = (seq_t *)realloc(cache, newsize * sizeof(seq_t));
    if (!cache) return 0;             /* out of memory – should not happen */

    /* initial values */
    const seq_t init[8] = { (seq_t)1, (seq_t)0, (seq_t)1, (seq_t)1,
                            (seq_t)0, (seq_t)3, (seq_t)1, (seq_t)3 };

    for (idx_t i = cached + 1; i <= n; ++i) {
        seq_t val;
        if (i <= 8) {
            val = init[i-1];
        } else {
            /* recurrence: a(i) = 2*a(i-2) + a(i-3) - a(i-4) - 2*a(i-5) + a(i-7) */
            seq_t pos = 2 * cache[i-3] + cache[i-4] + cache[i-8];
            seq_t neg = cache[i-5] + 2 * cache[i-6];
            val = pos - neg;               /* pos >= neg for all n ≥ 9 */
        }
        cache[i-1] = val;
    }

    cached = n;
    return cache[n-1];
}
