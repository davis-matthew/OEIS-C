/*
 * A002838  Balancing weights on the integer line.
 * https://oeis.org/A002838
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy, nice
 * Formula:   a(n) = [q^(n(n+1)/2)] of the Gaussian binomial [2n+1 choose n]_q
 *            = prod_{i=1..n} (1-q^(n+1+i))/(1-q^i), i.e. partitions of
 *            n(n+1)/2 into at most n parts each <= n+1
 * Range:     u8 n <= 6, u16 n <= 11, u32 n <= 20, u64 n <= 37, u128 n <= 70,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <string.h>

seq_t a(idx_t n) {
    long long N = (long long)n * ((long long)n + 1) / 2;
    long long nn = (long long)n;
    seq_t *c = (seq_t *)calloc((size_t)(N + 1), sizeof(seq_t));
    long long i, k;
    seq_t result;
    for (k = 0; k <= N; k++) c[k] = 0;
    c[0] = 1;
    for (i = 1; i <= nn; i++) {
        long long m = nn + 1 + i;
        /* divide by (1 - q^i) */
        for (k = i; k <= N; k++) c[k] += c[k - i];
        /* multiply by (1 - q^m) */
        if (m <= N) {
            for (k = N; k >= m; k--) c[k] -= c[k - m];
        }
    }
    result = c[N];
    free(c);
    return result;
}
