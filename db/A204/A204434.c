/*
 * A204434  Permanent of the n-th principal submatrix of A204433.
 * https://oeis.org/A204434
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = permanent of the n X n matrix M(i,j) = (2*(i+j+1)) mod 3,
 *            i,j = 1..n (the A204433 matrix). Rows (and columns) fall into 3
 *            classes by index mod 3, so a(n) is summed over 3x3
 *            class-transfer matrices K: prod(r_a!) * prod_b [c_b!/prod_a
 *            K_ab!] * 2^(#K on entries equal to 2)
 * Range:     u8 n <= 5, u16 n <= 8, u32 n <= 12, u64 n <= 20, u128 n <= 20,
 *            big any n
 * Validated: partial, pass, 20 terms through n = 20 (2026-11-07)
 * Helpers:   binomial
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include "../utils/binomial.h"

static seq_t A204434_fact(idx_t m) {
    seq_t r = 1;
    for (idx_t i = 2; i <= m; i++) r *= (seq_t)i;
    return r;
}

seq_t a(idx_t n) {
    idx_t cnt[3];
    cnt[0] = n / 3;
    cnt[1] = (n + 2) / 3;
    cnt[2] = (n + 1) / 3;
    /* value of entry for class pair (a,b): s=(a+b)%3: 0->2, 1->1, 2->0 */
    seq_t base = A204434_fact(cnt[0]) * A204434_fact(cnt[1]) * A204434_fact(cnt[2]);
    seq_t total = 0;
    idx_t K[3][3];
    for (idx_t k00 = 0; k00 <= cnt[0]; k00++) {
        for (idx_t k01 = 0; k00 + k01 <= cnt[0]; k01++) {
            idx_t k02 = cnt[0] - k00 - k01;
            if (k02 != 0) continue; /* entry (0,2) is 0 */
            for (idx_t k10 = 0; k10 <= cnt[1]; k10++) {
                for (idx_t k11 = 0; k10 + k11 <= cnt[1]; k11++) {
                    if (k11 != 0) continue; /* entry (1,1) is 0 */
                    idx_t k12 = cnt[1] - k10 - k11;
                    K[0][0] = k00; K[0][1] = k01; K[0][2] = k02;
                    K[1][0] = k10; K[1][1] = k11; K[1][2] = k12;
                    if (K[0][0] + K[1][0] > cnt[0]) continue;
                    if (K[0][1] + K[1][1] > cnt[1]) continue;
                    if (K[0][2] + K[1][2] > cnt[2]) continue;
                    K[2][0] = cnt[0] - K[0][0] - K[1][0];
                    K[2][1] = cnt[1] - K[0][1] - K[1][1];
                    K[2][2] = cnt[2] - K[0][2] - K[1][2];
                    if (K[2][0] != 0) continue; /* entry (2,0) is 0 */
                    if (K[2][0] + K[2][1] + K[2][2] != cnt[2]) continue;
                    seq_t term = base;
                    for (int b = 0; b < 3; b++) {
                        term *= binomial(cnt[b], K[0][b]);
                        term *= binomial(cnt[b] - K[0][b], K[1][b]);
                    }
                    idx_t e = K[0][0] + K[1][2] + K[2][1];
                    for (idx_t t = 0; t < e; t++) term *= 2;
                    total += term;
                }
            }
        }
    }
    return total;
}
