/*
 * A052731  E.g.f. [1-x -sqrt(1-2x-3x^2)]/(2x) - [1+x-sqrt(1-2x-3x^2)]/2 .
 * https://oeis.org/A052731
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, nonn
 * Formula:   Recurrence a(0)=a(1)=a(2)=0, a(3)=6 For n>=3:
 *            (-15n+15n³+15n²+3n⁴-18)·a(n) +(-n³-7n²-16n-12)·a(n+1)
 *            +(-3n²-16n-21)·a(n+2) +(n+4)·a(n+3)=0 The code memoises the
 *            terms and computes each new term using this recurrence
 * Range:     u8 n <= 4, u16 n <= 6, u32 n <= 10, u64 n <= 16, u128 n <= 18,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>

seq_t a(idx_t n)
{
    static seq_t *memo = NULL;
    static idx_t  memo_len = 0;
    static bool   initialized = false;

    /* initialise first four terms */
    if (!initialized) {
        memo_len = 4;
        memo = (seq_t *)malloc(memo_len * sizeof(seq_t));
        memo[0] = 0;
        memo[1] = 0;
        memo[2] = 0;
        memo[3] = 6;
        initialized = true;
    }

    /* extend memo table if necessary */
    if (n >= memo_len) {
        idx_t new_len = n + 1;
        memo = (seq_t *)realloc(memo, new_len * sizeof(seq_t));
        for (idx_t i = memo_len; i < new_len; ++i) memo[i] = 0;

        for (idx_t i = memo_len; i <= n; ++i) {
            if (i <= 3) continue;           /* base values already known */

            idx_t k = i - 3;                 /* current n in the recurrence  */
            long long k2 = (long long)k * k;                                   /* n²   */
            long long k3 = k2 * k;                                            /* n³   */
            long long k4 = k3 * k;                                            /* n⁴   */

            long long c0 = -15LL * (long long)k
                          + 15LL * k3
                          + 15LL * k2
                          + 3LL  * k4
                          - 18LL;

            long long c1 = -k3
                          - 7LL  * k2
                          - 16LL * (long long)k
                          - 12LL;

            long long c2 = -3LL * k2
                          - 16LL * (long long)k
                          - 21LL;

            long long c3 = (long long)k + 4LL;

#if defined(__cplusplus)
            /* C++ build – seq_t is an arbitrary‑precision class */
            seq_t term0 = memo[i-3] * (seq_t)c0;
            seq_t term1 = memo[i-2] * (seq_t)c1;
            seq_t term2 = memo[i-1] * (seq_t)c2;
            seq_t numerator = -term0 - term1 - term2;
            memo[i] = numerator / (seq_t)c3;
#else
            /* C build – use 128‑bit intermediate to avoid overflow */
            __int128 term0 = (__int128)memo[i-3] * c0;
            __int128 term1 = (__int128)memo[i-2] * c1;
            __int128 term2 = (__int128)memo[i-1] * c2;
            __int128 numerator = -term0 - term1 - term2;
            memo[i] = (seq_t)(numerator / c3);
#endif
        }

        memo_len = new_len;
    }

    return memo[n];
}
