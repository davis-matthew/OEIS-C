/*
 * A007254  McKay-Thompson series of class 6A for Monster.
 * https://oeis.org/A007254
 *
 * Offset:    -1    (a(-1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = [q^n] (t + 81/t) + 4*[n=0], where t =
 *            (eta(q)*eta(q^2)/(eta(q^3)*eta(q^6)))^4 = q^(-1) *
 *            Product_{k>=1, k==+-1 mod 6} (1-q^k)^4 * Product_{k>=1, k==+-2
 *            mod 6} (1-q^k)^8. Signed coefficients are kept as (positive
 *            part, negative part) pairs
 * Range:     u8 n <= 1, u16 n <= 6, u32 n <= 24, u64 n <= 89, u128 n <= 334,
 *            big any n
 * Helpers:   alloc_zero, expo, norm, release
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdbool.h>
#include "../utils/alloc_zero.h"
#include "../utils/release.h"
#include "../utils/expo.h"
#include "../utils/norm.h"

static seq_t *tP = 0, *tN = 0, *uP = 0, *uN = 0;
static idx_t cached = 0;

static void build(idx_t L)
{
    idx_t k, i;
    int e, j;
    if (tP) { release(tP, cached); release(tN, cached); release(uP, cached); release(uN, cached); }
    tP = alloc_zero(L); tN = alloc_zero(L);
    uP = alloc_zero(L); uN = alloc_zero(L);
    tP[0] = 1;
    uP[0] = 1;
    for (k = 1; k < L; k++) {
        e = expo(k);
        for (j = 0; j < e; j++) {
            /* multiply t-product by (1 - q^k) */
            for (i = L - 1; i >= k; i--) {
                seq_t np = tP[i] + tN[i - k];
                seq_t nn = tN[i] + tP[i - k];
                tP[i] = np;
                tN[i] = nn;
                norm(&tP[i], &tN[i]);
            }
            /* divide u-product by (1 - q^k) */
            for (i = k; i < L; i++) {
                uP[i] += uP[i - k];
                uN[i] += uN[i - k];
                norm(&uP[i], &uN[i]);
            }
        }
    }
    cached = L;
}

seq_t a(idx_t n)
{
    idx_t L;
    seq_t pos, neg;
    if (n < -1) return 0;
    L = n + 3;
    if (L > cached) {
        idx_t nl = cached * 2;
        if (nl < L) nl = L;
        if (nl < 16) nl = 16;
        build(nl);
    }
    pos = tP[n + 1];
    neg = tN[n + 1];
    if (n >= 1) {
        pos += uP[n - 1] * 81;
        neg += uN[n - 1] * 81;
    }
    if (n == 0) pos += 4;
    return pos - neg;
}
