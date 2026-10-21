/*
 * A070094  Number of acute integer triangles with perimeter n and relatively
 *          prime side lengths.
 * https://oeis.org/A070094
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 72, u16 n <= 72, u32 n <= 72, u64 n <= 72, u128 n <= 72,
 *            big any n
 * Helpers:   uc_gcd
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*
 * A070094: Number of acute integer triangles with perimeter n and relatively prime side lengths.
 * Computed by enumerating all integer triples (a,b,c) satisfying
 *   a ≤ b ≤ c
 *   a + b + c = n
 *   a + b > c   (triangle inequality)
 *   a^2 + b^2 > c^2   (acute angle condition)
 *   gcd(a,b,c) = 1    (primitive triangle)
 *
 * The function memoizes results in static storage to avoid recomputation.
 */
#include <stdlib.h>
#include <stdint.h>
#include "../utils/uc_gcd.h"

seq_t a(idx_t n)
{
    /* Allocate memoization tables on first use or when n exceeds current capacity */
    static seq_t *cache = NULL;
    static unsigned char *computed = NULL;
    static idx_t cap = 0;

    if (n == 0) return (seq_t)0;

    if (n >= cap) {
        idx_t newcap = n + 1;
        cache = (seq_t *)realloc(cache, newcap * sizeof(seq_t));
        computed = (unsigned char *)realloc(computed, newcap * sizeof(unsigned char));
        /* Mark new entries as not yet computed */
        for (idx_t i = cap; i < newcap; ++i) computed[i] = 0;
        cap = newcap;
    }

    if (computed[n]) return cache[n];  /* Return cached result */

    seq_t count = 0;
    idx_t max_a = n / 3;   /* a ≤ b ≤ c ⇒ a ≤ n/3 */

    for (idx_t a_val = 1; a_val <= max_a; ++a_val) {
        /* Triangle inequality a + b > c  ⇔  a + b > n/2 */
        idx_t min_b = (n / 2) - a_val + 1;
        if (min_b < a_val) min_b = a_val;

        idx_t max_b = (n - a_val) / 2;   /* b ≤ c  ⇔  b ≤ (n - a)/2 */

        if (min_b > max_b) continue;

        for (idx_t b_val = min_b; b_val <= max_b; ++b_val) {
            idx_t c_val = n - a_val - b_val;   /* since a+b+c = n */

            /* Acute condition: a^2 + b^2 > c^2 */
            unsigned long long ua = a_val;
            unsigned long long ub = b_val;
            unsigned long long uc = c_val;
            unsigned long long lhs = ua * ua + ub * ub;
            unsigned long long rhs = uc * uc;
            if (lhs <= rhs) continue;

            /* Primitive: gcd(a,b,c) == 1 */
            unsigned long long g = uc_gcd(ua, ub);
            g = uc_gcd(g, uc);
            if (g == 1) ++count;
        }
    }

    computed[n] = 1;
    cache[n] = count;
    return count;
}
