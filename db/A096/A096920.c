/*
 * A096920  Expansion of q^(-1/12) * eta(q^2)^4 / (eta(q)^2 * eta(q^4)) in
 *          powers of q.
 * https://oeis.org/A096920
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = Σ_{t ≤ n, t = k(k+1)/2} b(n - t) where b(m) is the number
 *            of partitions of m into distinct odd parts
 * Range:     u8 n <= 43, u16 n <= 128, u32 n <= 418, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>

seq_t a(idx_t n)
{
    /* compute b[0..n] : partitions into distinct odd parts */
    size_t N = (size_t)n + 1;
    seq_t *b = (seq_t *)calloc(N, sizeof(seq_t));
    if (!b) return (seq_t)0;            /* allocation failure, return 0 */
    b[0] = 1;
    for (idx_t p = 1; p <= n; p += 2) {
        for (idx_t i = n; i >= p; --i) {
            b[i] += b[i - p];
        }
    }

    /* compute a[0..n] using triangular numbers */
    seq_t *a_t = (seq_t *)calloc(N, sizeof(seq_t));
    if (!a_t) { free(b); return (seq_t)0; }
    for (idx_t i = 0; i <= n; ++i) {
        seq_t sum = 0;
        for (idx_t k = 0; ; ++k) {
            idx_t t = k * (k + 1) / 2;
            if (t > i) break;
            sum += b[i - t];
        }
        a_t[i] = sum;
    }

    seq_t result = a_t[n];

    free(b);
    free(a_t);
    return result;
}
