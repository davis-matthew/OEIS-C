/*
 * A000485  Number of partially labeled trees with n nodes (4 of which are
 *          labeled).
 * https://oeis.org/A000485
 *
 * Offset:    4    (a(4) is the first term)
 * Keywords:  nonn
 * Formula:   G.f.: A(x) = B(x)^4*(16-19*B(x)+6*B(x)^2)/(1-B(x))^5, B = g.f.
 *            of A000081. With H = B/(1-B) (so 1-B = 1/(1+H)) this is A(x) =
 *            B(x)*H(x)^3*(16 + 13*H(x) + 3*H(x)^2), which has only
 *            nonnegative coefficients. A000081: b(1) = 1, b(m) = (1/(m-1)) *
 *            Sum_{k=1..m-1} (Sum_{d|k} d*b(d)) * b(m-k)
 * Range:     u8 n <= 5, u16 n <= 9, u32 n <= 18, u64 n <= 37, u128 n <= 77,
 *            big any n
 * Helpers:   zarr
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include "../utils/zarr.h"

static seq_t *cacheA = NULL;
static idx_t cacheN = 0; /* cacheA valid for indices 0..cacheN-1 */

static void build(idx_t len) {
    seq_t *b = zarr(len);
    seq_t *s = zarr(len);
    seq_t *h = zarr(len);
    seq_t *h2 = zarr(len);
    seq_t *h3 = zarr(len);
    seq_t *q = zarr(len);
    seq_t *r = zarr(len);
    seq_t *A = zarr(len);
    idx_t i, j, d;

    /* A000081 */
    if (len > 1) {
        b[1] = 1;
        for (d = 1; d < len; d += 1) s[d] += (seq_t)1;
    }
    for (i = 2; i < len; i++) {
        seq_t sum = 0;
        for (j = 1; j < i; j++) sum += s[j] * b[i - j];
        b[i] = sum / (seq_t)(i - 1);
        for (d = i; d < len; d += i) s[d] += (seq_t)i * b[i];
    }

    /* H = B/(1-B): h[i] = b[i] + sum_{j=1}^{i-1} b[j]*h[i-j] */
    for (i = 1; i < len; i++) {
        seq_t sum = b[i];
        for (j = 1; j < i; j++) sum += b[j] * h[i - j];
        h[i] = sum;
    }

    /* H^2 */
    for (i = 2; i < len; i++) {
        seq_t sum = 0;
        for (j = 1; j < i; j++) sum += h[j] * h[i - j];
        h2[i] = sum;
    }
    /* H^3 */
    for (i = 3; i < len; i++) {
        seq_t sum = 0;
        for (j = 1; j + 2 <= i; j++) sum += h[j] * h2[i - j];
        h3[i] = sum;
    }
    /* Q = 16 + 13H + 3H^2 */
    for (i = 0; i < len; i++) q[i] = (seq_t)13 * h[i] + (seq_t)3 * h2[i];
    if (len > 0) q[0] += (seq_t)16;
    /* R = H^3 * Q */
    for (i = 3; i < len; i++) {
        seq_t sum = 0;
        for (j = 3; j <= i; j++) sum += h3[j] * q[i - j];
        r[i] = sum;
    }
    /* A = B * R */
    for (i = 4; i < len; i++) {
        seq_t sum = 0;
        for (j = 1; j + 3 <= i; j++) sum += b[j] * r[i - j];
        A[i] = sum;
    }

    free(b); free(s); free(h); free(h2); free(h3); free(q); free(r);
    if (cacheA) free(cacheA);
    cacheA = A;
    cacheN = len;
}

seq_t a(idx_t n) {
    if (n < 4) return 0;
    if (n >= cacheN) {
        idx_t len = n + 1;
        if (len < 2 * cacheN) len = 2 * cacheN;
        if (len < 32) len = 32;
        build(len);
    }
    return cacheA[n];
}
