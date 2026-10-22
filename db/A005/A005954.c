/*
 * A005954  Theta series of P_{10b} packing.
 * https://oeis.org/A005954
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = [t^n] ( E(t)^10 + 480*t*E(t)^6*G(t)^4 +
 *            1280*t^2*E(t)^2*G(t)^8 ), where E(t) = theta_3(t) = 1 +
 *            2*Sum_{k>=1} t^(k^2) and G(t) = Sum_{k>=0} t^(k*(k+1)).
 *            (Construction A on the (10,40,4) code with distance distribution
 *            1, A4=30, A8=5: E is the even-coordinate theta series and
 *            2*t^(1/4)*G is...
 * Range:     u8 n <= 0, u16 n <= 3, u32 n <= 10, u64 n <= 10, u128 n <= 10,
 *            big any n
 * Helpers:   alloc_zero, mul_E, mul_G, release
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include "../utils/alloc_zero.h"
#include "../utils/release.h"
#include "../utils/mul_E.h"
#include "../utils/mul_G.h"

static seq_t *res = NULL;
static idx_t res_len = 0;

static void build(idx_t len) {
    seq_t *P = alloc_zero(len);
    seq_t *Q = alloc_zero(len);
    seq_t *R = alloc_zero(len);
    idx_t i;
    int j;
    P[0] = 1;
    mul_E(P, len); mul_E(P, len);              /* E^2 */
    for (i = 0; i < len; i++) Q[i] = P[i];
    for (j = 0; j < 8; j++) mul_G(Q, len);     /* E^2 G^8 */
    for (j = 0; j < 4; j++) mul_E(P, len);     /* E^6 */
    for (i = 0; i < len; i++) R[i] = P[i];
    for (j = 0; j < 4; j++) mul_G(R, len);     /* E^6 G^4 */
    for (j = 0; j < 4; j++) mul_E(P, len);     /* E^10 */

    seq_t *out = alloc_zero(len);
    for (i = 0; i < len; i++) {
        seq_t v = P[i];
        if (i >= 1) v += (seq_t)480 * R[i - 1];
        if (i >= 2) v += (seq_t)1280 * Q[i - 2];
        out[i] = v;
    }
    release(P, len);
    release(Q, len);
    release(R, len);
    if (res) release(res, res_len);
    res = out;
    res_len = len;
}

seq_t a(idx_t n) {
    if (n >= res_len) {
        idx_t len = n + 1;
        if (len < 2 * res_len) len = 2 * res_len;
        if (len < 64) len = 64;
        build(len);
    }
    return res[n];
}
