/*
 * A007065  Number of M-sequences from multicomplexes on at most 5 variables
 *          with no monomial of degree greater than n.
 * https://oeis.org/A007065
 *
 * Offset:    -1    (a(-1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 1 + #{chains of M-sequence degrees}: write each h_d
 *            (degree d <= n) in Macaulay form, h_d = sum_i binomial(i+e_i, i)
 *            with 4 >= e_d >= e_{d-1} >= ... >= 0. This is coded as a tuple
 *            (c3,c2,c1,c0) of exponent multiplicities with c3+c2+c1+c0 <= d,
 *            or as the single top element binomial(d+4,4)....
 * Range:     u8 n <= 2, u16 n <= 4, u32 n <= 8, u64 n <= 13, u128 n <= 14,
 *            big any n
 * Helpers:   alloc_zero, release
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include <stdlib.h>
#include "../utils/alloc_zero.h"
#include "../utils/release.h"

seq_t a(idx_t n)
{
    if (n <= -1) return (seq_t)1;
    if (n == 0) return (seq_t)2;

    long long N = (long long)(n + 1) * (n + 2) / 2 * (n + 3) / 3 * (n + 4) / 4;
    seq_t *f = alloc_zero((idx_t)N);
    seq_t top = 1;
    long long k, c3, c2, c1, c0;

    /* degree 1: every tuple with sum <= 1 */
    k = 0;
    for (c3 = n; c3 >= 0; c3--)
        for (c2 = n - c3; c2 >= 0; c2--)
            for (c1 = n - c3 - c2; c1 >= 0; c1--)
                for (c0 = n - c3 - c2 - c1; c0 >= 0; c0--) {
                    if (c3 + c2 + c1 + c0 <= 1) f[k] = 1;
                    k++;
                }

    for (idx_t d = 1; d < n; d++) {
        seq_t S = 0;
        k = 0;
        for (c3 = n; c3 >= 0; c3--)
            for (c2 = n - c3; c2 >= 0; c2--)
                for (c1 = n - c3 - c2; c1 >= 0; c1--)
                    for (c0 = n - c3 - c2 - c1; c0 >= 0; c0--) {
                        long long s = c3 + c2 + c1 + c0;
                        if (s <= d) S += f[k];
                        if (s <= d + 1) f[k] = S + top;
                        k++;
                    }
    }

    seq_t res = 1;
    res += top;
    for (k = 0; k < N; k++) res += f[k];
    release(f, (idx_t)N);
    return res;
}
