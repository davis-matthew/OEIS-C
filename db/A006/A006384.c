/*
 * A006384  Number of sensed planar maps with n edges.
 * https://oeis.org/A006384
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, nice
 * Formula:   a(0) = 1; for n>0, a(n) = (1/2n)[A'(n) + Sum_{k<n,k|n}
 *            phi(n/k)*3^k*binomial(2k,k)] + q(n), where A'(n) = A000168(n) =
 *            2*3^n*binomial(2n,n)/((n+1)(n+2)), q(n) = (n+3)*A'((n-1)/2)/4
 *            for odd n, q(n) = (n-1)*A'((n-2)/2)/4 for even n. Equivalently
 *            2n*a(n) = A'(n) + Sum + E with E = (2m+1)(m+1)A'(m),...
 * Range:     u8 n <= 4, u16 n <= 7, u32 n <= 12, u64 n <= 22, u128 n <= 41,
 *            big any n
 * Helpers:   central, seq_is_fixed_width, sgcd64, sphi, spow3
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>
#include "../utils/seq_is_fixed_width.h"
#include "../utils/central.h"
#include "../utils/sgcd64.h"
#include "../utils/sphi.h"
#include "../utils/spow3.h"

static seq_t catalan(idx_t m) {
    return central(m) / (seq_t)(m + 1);
}

typedef struct { seq_t q, r; } qr_t;   /* value = q*D + r, 0 <= r < D */

static void qr_add(qr_t *acc, seq_t t, seq_t D) {
    acc->q += t / D;
    acc->r += t % D;
    if (acc->r >= D) { acc->r -= D; acc->q += 1; }
}

/* A'(m) = 2*3^m*Catalan(m)/(m+2), as quotient/remainder modulo D */
static qr_t aprime_qr(idx_t m, seq_t D) {
    qr_t res;
    seq_t x = 2 * catalan(m);
    uint64_t d = m + 2;
    uint64_t g = sgcd64((uint64_t)(x % (seq_t)d), d);
    x /= (seq_t)g;
    seq_t y = spow3(m) / (seq_t)(d / g);
    seq_t xr = x % D;
    seq_t t = xr * y;
    res.q = (x / D) * y + t / D;
    res.r = t % D;
    return res;
}

seq_t a(idx_t n) {
    if (n == 0) return 1;
    if (seq_is_fixed_width() && n > 100) return 0; /* far beyond any value that fits */

    seq_t D = (seq_t)2 * (seq_t)n;
    qr_t acc = aprime_qr(n, D);

    for (idx_t k = 1; k < n; k++) {
        if (n % k == 0) {
            seq_t t = (seq_t)sphi(n / k) * spow3(k) * central(k);
            qr_add(&acc, t, D);
        }
    }

    seq_t F = 0;
    if (n % 2 == 0) {
        idx_t m = (n - 2) / 2;
        qr_t e = aprime_qr(m, D);
        seq_t c = (seq_t)(2 * m + 1) * (seq_t)(m + 1);
        seq_t rc = e.r * c;
        qr_add(&acc, e.q * c * D, D);
        acc.q += e.q * c * 0; /* (kept for clarity: e.q*c*D contributes e.q*c to q) */
        qr_add(&acc, rc, D);
    } else {
        idx_t m = (n - 1) / 2;
        F = spow3(m) * catalan(m);
    }

    return acc.q + acc.r / D + F;
}
