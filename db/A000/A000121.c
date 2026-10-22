/*
 * A000121  Number of representations of n as a sum of Fibonacci numbers (1 is
 *          allowed twice as a part).
 * https://oeis.org/A000121
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = number of partitions of n into distinct parts from the
 *            multiset {1, 1, 2, 3, 5, 8, 13, ...} (Fibonacci numbers, 1
 *            allowed twice), computed by 0/1-knapsack counting; a(0) = 1.
 *            Equivalently a(n) = A000119(n) + A000119(n-1) for n >= 1
 * Range:     u8 n <= 6765, u16 n <= 6765, u32 n <= 6765, u64 n <= 6765,
 *            u128 n <= 6765, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>

seq_t a(idx_t n)
{
    uint64_t N = (uint64_t)n;
    uint64_t *cnt = (uint64_t *)calloc((size_t)N + 1, sizeof(uint64_t));
    uint64_t f1, f2, p, m, t;
    seq_t r;
    int k;

    if (!cnt) return (seq_t)0;
    cnt[0] = 1;

    /* the part 1 used twice (as two distinct Fibonacci numbers F(1), F(2)) */
    for (k = 0; k < 2; k++) {
        if (N >= 1) {
            for (m = N; m >= 1; m--) cnt[m] += cnt[m - 1];
        }
    }

    /* remaining Fibonacci parts 2, 3, 5, 8, ... each used at most once */
    f1 = 1; f2 = 2;
    while (f2 <= N) {
        p = f2;
        for (m = N; m >= p; m--) {
            cnt[m] += cnt[m - p];
            if (m == p) break;
        }
        t = f1 + f2;
        f1 = f2;
        f2 = t;
    }

    r = (seq_t)cnt[N];
    free(cnt);
    return r;
}
