/*
 * A233421  Let m = n-th nonsquare = A000037(n); then a(n) = A006255(m).
 * https://oeis.org/A233421
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = A006255(A000037(n)), with A000037(n) = n + floor(1/2 +
 *            sqrt(n)); A006255 computed by GF(2) linear-algebra search over
 *            exponent-parity vectors
 * Range:     u8 n <= 119, u16 n <= 5000, u32 n <= 5000, u64 n <= 5000,
 *            u128 n <= 5000, big any n
 * Validated: standard, pass, 78 terms through n = 4889 (2026-11-07)
 * Helpers:   isqrt_u64
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include "../utils/isqrt_u64.h"

static int A006255_reduce_vec(uint64_t *v, uint64_t **basis, int W) {
    int w = W - 1;
    while (w >= 0) {
        if (!v[w]) { w--; continue; }
        int b = 63 - __builtin_clzll(v[w]);
        int h = w * 64 + b;
        if (!basis[h]) return h;
        {
            uint64_t *bv = basis[h];
            int i;
            for (i = 0; i <= w; i++) v[i] ^= bv[i];
        }
    }
    return -1;
}

static void A006255_set_vec(uint64_t x, const uint32_t *spf, const int *pidx, uint64_t *v, int W) {
    memset(v, 0, (size_t)W * sizeof(uint64_t));
    while (x > 1) {
        uint32_t p = spf[x];
        int e = 0;
        while (x % p == 0) { x /= p; e++; }
        if (e & 1) {
            int k = pidx[p];
            v[k >> 6] ^= (uint64_t)1 << (k & 63);
        }
    }
}

static uint64_t A006255_a(uint64_t n) {
    uint64_t L, i, j, m, result = 0;
    uint32_t *spf;
    int *pidx;
    int P = 0, W, used = 0;
    uint64_t *vn, *vm, *t, *pool;
    uint64_t **basis;

    if (n <= 1) return 1;
    L = 4 * n + 16;

    spf = (uint32_t *)calloc((size_t)(L + 1), sizeof(uint32_t));
    pidx = (int *)calloc((size_t)(L + 1), sizeof(int));
    for (i = 2; i <= L; i++) {
        if (spf[i] == 0) {
            pidx[i] = P++;
            for (j = i; j <= L; j += i)
                if (spf[j] == 0) spf[j] = (uint32_t)i;
        }
    }
    W = (P + 63) / 64;

    vn = (uint64_t *)malloc((size_t)W * sizeof(uint64_t));
    vm = (uint64_t *)malloc((size_t)W * sizeof(uint64_t));
    t = (uint64_t *)malloc((size_t)W * sizeof(uint64_t));
    pool = (uint64_t *)malloc((size_t)P * (size_t)W * sizeof(uint64_t));
    basis = (uint64_t **)calloc((size_t)P, sizeof(uint64_t *));

    A006255_set_vec(n, spf, pidx, vn, W);
    {
        bool zero = true;
        int k;
        for (k = 0; k < W; k++) if (vn[k]) { zero = false; break; }
        if (zero) result = n;
    }

    for (m = n + 1; result == 0 && m <= L; m++) {
        int k, h;
        A006255_set_vec(m, spf, pidx, vm, W);
        for (k = 0; k < W; k++) t[k] = vn[k] ^ vm[k];
        if (A006255_reduce_vec(t, basis, W) < 0) { result = m; break; }
        h = A006255_reduce_vec(vm, basis, W);
        if (h >= 0) {
            uint64_t *slot = pool + (size_t)used * (size_t)W;
            memcpy(slot, vm, (size_t)W * sizeof(uint64_t));
            basis[h] = slot;
            used++;
        }
    }

    free(spf); free(pidx); free(vn); free(vm); free(t); free(pool); free(basis);
    return result;
}

static uint64_t A000037_a(uint64_t n) {
    uint64_t r = isqrt_u64(n);
    uint64_t k = r;
    if (r * (r + 1) < n) k = k + 1;
    return n + k;
}

seq_t a(idx_t n)
{
    uint64_t m = A000037_a((uint64_t)n);
    return (seq_t)A006255_a(m);
}
