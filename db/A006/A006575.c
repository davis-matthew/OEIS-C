/*
 * A006575  Number of primitive (aperiodic, or Lyndon) asymmetric rhythm
 *          cycles: ones having no nontrivial shift automorphism.
 * https://oeis.org/A006575
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = (Sum_{d|n, d odd} mu(d)*(3^(n/d)-1))/(2*n)
 * Range:     u8 n <= 7, u16 n <= 13, u32 n <= 23, u64 n <= 44, u128 n <= 85,
 *            big any n
 * Helpers:   bn_pow3
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include "../utils/bn_pow3.h"

static int mobius_u64(uint64_t n) {
    int mu = 1;
    uint64_t p;
    if (n == 1) return 1;
    for (p = 2; p * p <= n; p++) {
        if (n % p == 0) {
            n /= p;
            if (n % p == 0) return 0;
            mu = -mu;
        }
    }
    if (n > 1) mu = -mu;
    return mu;
}

/* little-endian 32-bit limb big integers of fixed length L */
static void bn_add(uint32_t *x, const uint32_t *y, size_t L) {
    uint64_t c = 0;
    size_t i;
    for (i = 0; i < L; i++) {
        c += (uint64_t)x[i] + y[i];
        x[i] = (uint32_t)c;
        c >>= 32;
    }
}

static void bn_add_small(uint32_t *x, uint64_t v, size_t L) {
    size_t i;
    uint64_t c = v;
    for (i = 0; i < L && c; i++) {
        c += x[i];
        x[i] = (uint32_t)c;
        c >>= 32;
    }
}

static void bn_sub(uint32_t *x, const uint32_t *y, size_t L) {
    int64_t b = 0;
    size_t i;
    for (i = 0; i < L; i++) {
        int64_t t = (int64_t)x[i] - (int64_t)y[i] - b;
        if (t < 0) { t += ((int64_t)1 << 32); b = 1; } else b = 0;
        x[i] = (uint32_t)t;
    }
}

static void bn_div_small(uint32_t *x, uint64_t dv, size_t L) {
    uint64_t r = 0;
    size_t i = L;
    while (i > 0) {
        uint64_t cur;
        i--;
        cur = (r << 32) | x[i];
        x[i] = (uint32_t)(cur / dv);
        r = cur % dv;
    }
}

seq_t a(idx_t n) {
    size_t L = (size_t)(n / 20) + 6;
    uint32_t *pos = (uint32_t *)calloc(L, sizeof(uint32_t));
    uint32_t *neg = (uint32_t *)calloc(L, sizeof(uint32_t));
    uint32_t *tmp = (uint32_t *)calloc(L, sizeof(uint32_t));
    uint64_t d;
    size_t i;
    seq_t res = 0;

    for (d = 1; d <= n; d += 2) {
        int mu;
        if (n % d != 0) continue;
        mu = mobius_u64(d);
        if (mu == 0) continue;
        bn_pow3(tmp, n / d, L);
        if (mu > 0) {
            bn_add(pos, tmp, L);
            bn_add_small(neg, 1, L);
        } else {
            bn_add(neg, tmp, L);
            bn_add_small(pos, 1, L);
        }
    }
    bn_sub(pos, neg, L);
    bn_div_small(pos, 2 * (uint64_t)n, L);

    i = L;
    while (i > 0) {
        int sh;
        i--;
        for (sh = 24; sh >= 0; sh -= 8) {
            res = res * 256 + (seq_t)((pos[i] >> sh) & 0xFFu);
        }
    }

    free(pos);
    free(neg);
    free(tmp);
    return res;
}
