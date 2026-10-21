/*
 * A203050  Number of nX1 0..5 arrays with every nonzero element less than or
 *          equal to some horizontal or vertical neighbor.
 * https://oeis.org/A203050
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(1)=1, a(2)=6, a(3)=36, a(4)=161, a(5)=636, a(6)=2507,
 *            a(7)=10213, a(8)=42182, a(9)=173606, a(10)=710976,
 *            a(11)=2908797. For n>11:
 *            a(n)=6*a(n-1)-15*a(n-2)+35*a(n-3)-35*a(n-4)+56*a(n-5)-28*a(n-6)
 *            +36*a(n-7)-9*a(n-8)+10*a(n-9)-a(n-10)+a(n-11)
 * Range:     u8 n <= 4, u16 n <= 8, u32 n <= 16, u64 n <= 31, u128 n <= 63,
 *            big any n
 * Helpers:   compute_initial_terms
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include "../utils/compute_initial_terms.h"

seq_t a(idx_t n)
{
    if (n == 0) return 0;                     /* out of range, unused */

    /* maximum number of precomputed terms (enough for n up to 10000) */
    #define MAX_CACHE 20000

    static seq_t cache[MAX_CACHE];
    static size_t cache_len = 0;

    if (cache_len == 0) {
        seq_t init[11];
        compute_initial_terms(init);
        for (size_t i = 0; i < 11; ++i)
            cache[i] = init[i];
        cache_len = 11;
    }

    if ((size_t)n <= cache_len)
        return cache[n - 1];

    /* compute missing terms up to n */
    for (size_t i = cache_len; i < (size_t)n; ++i) {
        /* recurrence for a(i+1) using a(i)...a(i-10) and a(i-11) */
        seq_t val = (seq_t)6   * cache[i - 1]
                  - (seq_t)15  * cache[i - 2]
                  + (seq_t)35  * cache[i - 3]
                  - (seq_t)35  * cache[i - 4]
                  + (seq_t)56  * cache[i - 5]
                  - (seq_t)28  * cache[i - 6]
                  + (seq_t)36  * cache[i - 7]
                  - (seq_t)9   * cache[i - 8]
                  + (seq_t)10  * cache[i - 9]
                  - cache[i -10]
                  + cache[i -11];
        cache[i] = val;
    }
    cache_len = (size_t)n;
    return cache[n - 1];
}
