/*
 * A108012  a(n)= 8*a(n-1) -16*a(n-2) +4*a(n-4).
 * https://oeis.org/A108012
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 8*a(n-1) - 16*a(n-2) + 4*a(n-4) with a(0)=0, a(1)=4,
 *            a(2)=18, a(3)=76
 * Range:     u8 n <= 3, u16 n <= 7, u32 n <= 15, u64 n <= 22, u128 n <= 22,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stddef.h>

seq_t a(idx_t n)
{
    /* static array holding already computed terms up to a limited size
       (the problem guarantees n ≤ 10000, so 10001 elements are ample) */
    static seq_t dp[10001];
    /* number of terms computed and stored (initially 4: indices 0‑3) */
    static idx_t computed = 4;

    /* one‑time initialization of the base cases */
    if (computed == 4) {
        dp[0] = (seq_t)0;
        dp[1] = (seq_t)4;
        dp[2] = (seq_t)18;
        dp[3] = (seq_t)76;
    }

    /* if the requested index is already computed, return it */
    if (n < computed)
        return dp[(size_t)n];

    /* compute missing terms up to n */
    for (idx_t i = computed; i <= n; ++i) {
        seq_t t1 = dp[i - 1] * (seq_t)8;
        seq_t t2 = dp[i - 2] * (seq_t)16;
        seq_t t3 = dp[i - 4] * (seq_t)4;
        dp[i] = t1 - t2 + t3;
    }
    computed = n + 1;
    return dp[(size_t)n];
}
