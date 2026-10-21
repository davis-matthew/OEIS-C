/*
 * A222731  Total sum of parts of multiplicity 3 in all partitions of n.
 * https://oeis.org/A222731
 *
 * Offset:    3    (a(3) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = sum_{t=1}^{⌊n/3⌋} t·p(n-3t) - sum_{t=1}^{⌊n/4⌋}
 *            t·p(n-4t), where p(k) is the ordinary partition function (number
 *            of unrestricted partitions of k). The recurrence for p(k) uses
 *            generalized pentagonal numbers and only plus/minus operations.
 *            This implementation works for the 8/16/32/64‑bit bu...
 * Range:     u8 n <= 18, u16 n <= 41, u32 n <= 116, u64 n <= 377,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#ifdef __cplusplus
#include <new>
#endif

seq_t a(idx_t n)
{
    if (n < 3) return (seq_t)0;

    size_t N = (size_t)n;
    seq_t *p;

#ifdef __cplusplus
    /* value‑initialize the array (zero) */
    p = new seq_t[N + 1]();   /* throws on allocation failure */
#else
    p = (seq_t *)calloc(N + 1, sizeof(seq_t));
    if (!p) return (seq_t)0;
#endif

    p[0] = (seq_t)1;

    /* compute partition numbers p[0..N] by the pentagonal‑number recurrence */
    for (size_t k = 1; k <= N; ++k) {
        seq_t pos = 0, neg = 0;
        for (size_t m = 1;; ++m) {
            /* generalized pentagonal numbers: m(3m±1)/2 */
            size_t gp1 = m * (3 * m - 1) / 2;
            if (gp1 > k) break;
            if (m & 1) pos += p[k - gp1];
            else       neg += p[k - gp1];

            size_t gp2 = m * (3 * m + 1) / 2;
            if (gp2 > k) continue;   /* gp2 ≥ gp1, so if gp1≤k still check gp2 */
            if (m & 1) pos += p[k - gp2];
            else       neg += p[k - gp2];
        }
        p[k] = pos - neg;            /* pos ≥ neg always */
    }

    /* compute a(n) from the convolution formula */
    seq_t ans = 0;
    for (size_t t = 1; t <= N / 3; ++t)
        ans += (seq_t)t * p[N - 3 * t];
    for (size_t t = 1; t <= N / 4; ++t)
        ans -= (seq_t)t * p[N - 4 * t];

#ifdef __cplusplus
    delete[] p;
#else
    free(p);
#endif

    return ans;
}
