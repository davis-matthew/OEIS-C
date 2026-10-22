/*
 * A005802  Number of permutations in S_n with longest increasing subsequence
 *          of length <= 3 (i.e., 1234-avoiding permutations); vexillary
 *          permutations (i.e., 2143-avoiding).
 * https://oeis.org/A005802
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, nice
 * Formula:   a(0) = 1, a(1) = 1, (n^2 + 8*n + 16)*a(n + 2) = (10*n^2 + 42*n +
 *            41)*a(n + 1) - (9*n^2 + 18*n + 9)*a(n). To avoid overflow,
 *            a(n+1) and a(n) are each split as d*q + r with d = (n+4)^2. The
 *            exact division then only happens on a small remainder term
 * Range:     u8 n <= 5, u16 n <= 8, u32 n <= 14, u64 n <= 25, u128 n <= 46,
 *            big any n
 * Helpers:   grow_zero, seq_is_big, zalloc_arr
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>
#include "../utils/seq_is_big.h"
#include "../utils/zalloc_arr.h"
#include "../utils/grow_zero.h"

seq_t a(idx_t n) {
    static seq_t *memo = NULL;
    static idx_t cap = 0;
    static idx_t done = 0;

    if (memo == NULL) {
        cap = 64;
        memo = zalloc_arr(cap);
        memo[0] = 1;
        memo[1] = 1;
        done = 1;
    }
    if (n <= done) return memo[n];

    if (n >= cap) {
        idx_t ncap = cap * 2;
        if (ncap <= n) ncap = n + 64;
        memo = grow_zero(memo, cap, ncap);
        cap = ncap;
    }

    for (idx_t i = done + 1; i <= n; ++i) {
        uint64_t k = (uint64_t)(i - 2);
        uint64_t c1 = 10 * k * k + 42 * k + 41;
        uint64_t c2 = 9 * (k + 1) * (k + 1);
        uint64_t d = (k + 4) * (k + 4);
        seq_t a1 = memo[i - 1];
        seq_t a0 = memo[i - 2];
        seq_t dd = (seq_t)d;

        if (!seq_is_big() && (long long)dd != (long long)d) {
            /* fixed-width build: values here no longer fit anyway */
            memo[i] = 0;
            continue;
        }

        seq_t q1 = a1 / dd;
        seq_t q0 = a0 / dd;
        long long r1 = (long long)(a1 - q1 * dd);
        long long r0 = (long long)(a0 - q0 * dd);

        seq_t q = (seq_t)c1 * q1 - (seq_t)c2 * q0;
        __int128 t = (__int128)c1 * (__int128)r1 - (__int128)c2 * (__int128)r0;
        t /= (__int128)d;
        if (t >= 0)
            q += (seq_t)(uint64_t)t;
        else
            q -= (seq_t)(uint64_t)(-t);

        memo[i] = q;
    }
    done = n;
    return memo[n];
}
