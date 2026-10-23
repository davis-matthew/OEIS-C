/*
 * A120525  First differences of successive generalized meta-Fibonacci numbers
 *          A120503.
 * https://oeis.org/A120525
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 1 if n‑1 can be written as * Σ t_i · (3^i – 1)/2 with
 *            each t_i ∈ {0,1,2}, otherwise 0. * This follows from the
 *            generating function * D(z) = z · ∏_{i≥1} (1 + z^{[i]} +
 *            z^{2[i]}), [i] = (3^i – 1)/2
 * Range:     u8 n <= 75, u16 n <= 75, u32 n <= 75, u64 n <= 75, u128 n <= 75,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

seq_t a(idx_t n)
{
    /* m is the exponent we need to represent */
    seq_t m = (seq_t)n - 1;

    /* Pre‑compute the weights [i] = (3^i – 1)/2 using w = 3*w + 1.
       64 slots are more than enough for any reasonable n (≤2^64). */
    seq_t weight[64];
    int wcnt = 0;
    seq_t w = 1;
    while (w <= m) {
        weight[wcnt++] = w;
        w = 3 * w + 1;
        if (wcnt >= 64) break;   /* safety against pathological n */
    }

    /* Greedy test from largest weight to smallest */
    for (int i = wcnt - 1; i >= 0; --i) {
        seq_t t = m / weight[i];
        if (t > (seq_t)2) return 0;          /* digit too large → not a leaf */
        m -= t * weight[i];
    }
    return (m == 0) ? 1 : 0;   /* representation succeeded */
}
