/*
 * A003444  Number of dissections of a polygon.
 * https://oeis.org/A003444
 *
 * Offset:    4    (a(4) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = (1/(2m)) Sum_{d | gcd(2m, k)} phi(d)*binomial(2m/d, k/d),
 *            with m = n-1, k = m-2 = n-3 (necklaces with 2m beads, k of them
 *            black; the formula's index is shifted by 1 to match the OEIS
 *            offset)
 * Range:     u8 n <= 8, u16 n <= 12, u32 n <= 21, u64 n <= 37, u128 n <= 70,
 *            big any n
 * Helpers:   a3444_binom, a3444_gcd, a3444_phi
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>
#include "../utils/a3444_binom.h"
#include "../utils/a3444_gcd.h"
#include "../utils/a3444_phi.h"

seq_t a(idx_t n) {
    idx_t m, N, k, g;
    if (n < 4) return 0;
    m = n - 1;
    N = 2 * m;
    k = n - 3;
    g = a3444_gcd(N, k);

    if (m <= 300) {
        /* Pascal's triangle with each entry stored as q*N + r (0 <= r < N),
           so the result is exact even when the binomials overflow seq_t */
        seq_t q[302];
        uint64_t r[302];
        seq_t Q = 0;
        uint64_t R = 0;
        idx_t row, j;
        for (j = 0; j <= k; j++) { q[j] = 0; r[j] = 0; }
        r[0] = 1;
        for (row = 1; row <= N; row++) {
            idx_t top = row < k ? row : k;
            for (j = top; j >= 1; j--) {
                q[j] += q[j - 1];
                r[j] += r[j - 1];
                if (r[j] >= N) { r[j] -= N; q[j] += 1; }
            }
            if (N % row == 0) {
                idx_t d = N / row;
                if (g % d == 0) {
                    uint64_t ph = a3444_phi(d);
                    idx_t c = k / d;
                    Q += (seq_t)ph * q[c];
                    R += ph * r[c];
                }
            }
        }
        return Q + (seq_t)(R / N);
    } else {
        seq_t sum = 0;
        uint64_t d;
        for (d = 1; d * d <= g; d++) {
            if (g % d == 0) {
                sum += (seq_t)a3444_phi(d) * a3444_binom(N / d, k / d);
                if (d != g / d) {
                    uint64_t e = g / d;
                    sum += (seq_t)a3444_phi(e) * a3444_binom(N / e, k / e);
                }
            }
        }
        return sum / (seq_t)N;
    }
}
