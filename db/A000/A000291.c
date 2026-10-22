/*
 * A000291  Number of bipartite partitions of n white objects and 2 black
 *          ones.
 * https://oeis.org/A000291
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = A000070(n) + A000097(n), where A000070(n) = Sum_{k=0..n}
 *            p(k) and A000097(n) = Sum_{j=0..floor(n/2)} A000070(n-2j); p(k)
 *            (A000041) computed by Euler's pentagonal number recurrence, p(0)
 *            = 1
 * Range:     u8 n <= 8, u16 n <= 27, u32 n <= 94, u64 n <= 339,
 *            u128 n <= 1278, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n)
{
    long long N = (long long)n;
    long long m, k, i;
    seq_t *p = (seq_t *)calloc((size_t)(N + 1), sizeof(seq_t));
    seq_t result;

    p[0] = 1;
    for (m = 1; m <= N; m++) {
        seq_t pos = 0, neg = 0;
        for (k = 1; ; k++) {
            long long g1 = k * (3 * k - 1) / 2;
            long long g2 = k * (3 * k + 1) / 2;
            if (g1 > m) break;
            if (k & 1) pos += p[m - g1]; else neg += p[m - g1];
            if (g2 <= m) {
                if (k & 1) pos += p[m - g2]; else neg += p[m - g2];
            }
        }
        p[m] = pos - neg;
    }

    /* prefix sums: p[i] becomes A000070(i) */
    for (i = 1; i <= N; i++)
        p[i] += p[i - 1];

    result = p[N];
    for (i = N; ; i -= 2) {
        result += p[i];
        if (i < 2) break;
    }

    free(p);
    return result;
}
