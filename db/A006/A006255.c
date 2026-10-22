/*
 * A006255  R. L. Graham's sequence: a(n) = smallest m for which there is a
 *          sequence n = b_1 < b_2 < ... < b_t = m such that b_1*b_2*...*b_t
 *          is a perfect square.
 * https://oeis.org/A006255
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, nice
 * Formula:   search: a(n) = smallest m > n such that v(n)+v(m) lies in the
 *            GF(2)-span of v(n+1),...,v(m-1), where v(k) is the
 *            exponent-parity vector of k (a(n) = n if n is a square)
 * Range:     u8 n <= 130, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   reduce_vec, set_vec
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include "../utils/reduce_vec.h"
#include "../utils/set_vec.h"

seq_t a(idx_t n_idx) {
    uint64_t n = (uint64_t)n_idx;
    uint64_t L, i, j, m, result = 0;
    uint32_t *spf;
    int *pidx;
    int P = 0, W, used = 0;
    uint64_t *vn, *vm, *t, *pool;
    uint64_t **basis;

    if (n <= 1) return (seq_t)1;
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

    set_vec(n, spf, pidx, vn, W);
    {
        bool zero = true;
        int k;
        for (k = 0; k < W; k++) if (vn[k]) { zero = false; break; }
        if (zero) result = n;
    }

    for (m = n + 1; result == 0 && m <= L; m++) {
        int k, h;
        set_vec(m, spf, pidx, vm, W);
        for (k = 0; k < W; k++) t[k] = vn[k] ^ vm[k];
        if (reduce_vec(t, basis, W) < 0) { result = m; break; }
        h = reduce_vec(vm, basis, W);
        if (h >= 0) {
            uint64_t *slot = pool + (size_t)used * (size_t)W;
            memcpy(slot, vm, (size_t)W * sizeof(uint64_t));
            basis[h] = slot;
            used++;
        }
    }

    free(spf); free(pidx); free(vn); free(vm); free(t); free(pool); free(basis);
    return (seq_t)result;
}
