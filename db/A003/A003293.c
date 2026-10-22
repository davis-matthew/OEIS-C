/*
 * A003293  Number of planar partitions of n decreasing across rows.
 * https://oeis.org/A003293
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy, nice
 * Formula:   Euler transform of c(k) = ceil(k/2): a(0) = 1, n*a(n) =
 *            Sum_{k=1..n} b(k)*a(n-k), where b(k) = Sum_{d|k} d*ceil(d/2).
 *            Exact multi-limb integers (32-bit limbs) are used internally
 * Range:     u8 n <= 11, u16 n <= 25, u32 n <= 62, u64 n <= 162,
 *            u128 n <= 435, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>
#include <string.h>

static uint32_t **a3293_V = NULL;
static size_t *a3293_VL = NULL;
static idx_t a3293_AL = 0, a3293_CAP = 0;

static void a3293_extend(idx_t n) {
    idx_t L = n + 1, d, m, k;
    uint64_t *b;
    if (L <= a3293_AL) return;
    if (L > a3293_CAP) {
        idx_t nc = a3293_CAP ? a3293_CAP : 64;
        while (nc < L) nc *= 2;
        a3293_V = (uint32_t **)realloc(a3293_V, (size_t)nc * sizeof(uint32_t *));
        a3293_VL = (size_t *)realloc(a3293_VL, (size_t)nc * sizeof(size_t));
        a3293_CAP = nc;
    }
    b = (uint64_t *)calloc((size_t)L, sizeof(uint64_t));
    for (d = 1; d < L; d++) {
        uint64_t w = (uint64_t)d * ((d + 1) / 2);
        for (m = d; m < L; m += d) b[m] += w;
    }
    for (m = a3293_AL; m < L; m++) {
        if (m == 0) {
            a3293_V[0] = (uint32_t *)malloc(sizeof(uint32_t));
            a3293_V[0][0] = 1;
            a3293_VL[0] = 1;
            continue;
        }
        {
            size_t W = a3293_VL[m - 1] + 3, i, len;
            uint64_t *acc = (uint64_t *)calloc(W + 1, sizeof(uint64_t));
            uint32_t *out = (uint32_t *)malloc(W * sizeof(uint32_t));
            uint64_t carry = 0, r = 0;
            for (k = 1; k <= m; k++) {
                uint64_t bk = b[k];
                const uint32_t *x = a3293_V[m - k];
                size_t lx = a3293_VL[m - k];
                for (i = 0; i < lx; i++) {
                    uint64_t p = (uint64_t)x[i] * bk;
                    acc[i] += (uint32_t)p;
                    acc[i + 1] += p >> 32;
                }
            }
            for (i = 0; i < W; i++) {
                uint64_t t = acc[i] + carry;
                out[i] = (uint32_t)t;
                carry = t >> 32;
            }
            for (i = W; i-- > 0;) {
                uint64_t cur = (r << 32) | out[i];
                out[i] = (uint32_t)(cur / m);
                r = cur % m;
            }
            len = W;
            while (len > 1 && out[len - 1] == 0) len--;
            free(acc);
            a3293_V[m] = out;
            a3293_VL[m] = len;
        }
    }
    free(b);
    a3293_AL = L;
}

seq_t a(idx_t n) {
    seq_t v = 0;
    const uint32_t *x;
    size_t i;
    a3293_extend(n);
    x = a3293_V[n];
    for (i = a3293_VL[n]; i-- > 0;) {
        v <<= 16;
        v += (idx_t)(x[i] >> 16);
        v <<= 16;
        v += (idx_t)(x[i] & 0xffffu);
    }
    return v;
}
