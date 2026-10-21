/*
 * A203645  Number of arrays of n 0..14 integers with new values introduced in
 *          order 0..14 but otherwise unconstrained.
 * https://oeis.org/A203645
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = Σ_{k=1}^{min(n,15)} S(n,k), where S(n,k) are Stirling
 *            numbers of the second kind (S(0,0)=1, S(n,0)=0 for n>0, S(n,k)=0
 *            for k>n). This follows from the fact that an array obeying the
 *            “new values introduced in order 0..14” rule corresponds to a
 *            partition of the position set into at most 15 n...
 * Range:     u8 n <= 6, u16 n <= 9, u32 n <= 15, u64 n <= 25, u128 n <= 42,
 *            big any n
 * Helpers:   ensure_capacity
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdint.h>
#include "../utils/ensure_capacity.h"

seq_t a(idx_t n)
{
    /* Cache for computed terms */
    static seq_t *cache = NULL;
    static size_t cache_cap = 0;
    static size_t computed_up_to = 0;

    if (n == 0) return 0; /* a(0) is outside the offset, not used */

    /* Allocate cache enough to hold a[0..n] */
    if ((size_t)n + 1 > cache_cap)
        ensure_capacity((size_t)n + 1, &cache_cap, (void **)&cache,
                        sizeof(seq_t));

    /* Return cached value if already computed */
    if (n <= computed_up_to)
        return cache[n];

    /* Stirling numbers recurrence: previous row stored in s[0..15] */
    static seq_t s[16];
    static seq_t t[16];
    static idx_t s_n = 0;           /* n for which s[] holds S(s_n,k) */

    /* Initialise for n = 0 (S(0,0)=1, others 0) */
    if (computed_up_to == 0 && s_n == 0 && s[0] == 0) {
        for (int i = 0; i < 16; ++i) s[i] = 0;
        s[0] = 1;
    }

    /* Compute rows from computed_up_to+1 up to n */
    for (idx_t cur = computed_up_to + 1; cur <= n; ++cur) {
        int maxk = cur < 15 ? (int)cur : 15;   /* k runs up to 15 */

        t[0] = 0;      /* S(cur,0)=0 for cur>0 */
        for (int k = 1; k <= maxk; ++k) {
            /* S(cur,k) = S(cur-1,k-1) + k * S(cur-1,k) */
            t[k] = s[k - 1] + (seq_t)k * s[k];
        }
        for (int k = maxk + 1; k < 16; ++k)
            t[k] = 0;   /* S(cur,k)=0 for k>cur or k>15 */

        /* Sum S(cur,k) for k=1..maxk to obtain a(cur) */
        seq_t sum = 0;
        for (int k = 1; k <= maxk; ++k)
            sum += t[k];
        cache[cur] = sum;

        /* Prepare for next iteration */
        for (int k = 0; k < 16; ++k)
            s[k] = t[k];
        s_n = cur;
    }

    computed_up_to = n;
    return cache[n];
}
