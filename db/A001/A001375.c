/*
 * A001375  Relational systems on n nodes.
 * https://oeis.org/A001375
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Sum_{1*s_1+2*s_2+...=n} fix
 *            A[s_1,s_2,...]/(1^s_1*s_1!*2^s_2*s_2!*...), fix A[s_1,s_2,...] =
 *            8^Sum_{i,j>=1} gcd(i,j)*s_i*s_j. Computed as (1/n!) * Sum over
 *            cycle types of (class size) * 8^exponent
 * Range:     u8 n <= 1, u16 n <= 2, u32 n <= 3, u64 n <= 4, u128 n <= 6,
 *            big any n
 * Validated: partial, pass, 8 terms through n = 8 (2026-11-01)
 * Helpers:   gcd_u64
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>
#include "../utils/gcd_u64.h"

static seq_t r1375_pow8(uint64_t e) {
    seq_t r = (seq_t)1, b = (seq_t)8;
    while (e) {
        if (e & 1) r *= b;
        e >>= 1;
        if (e) b *= b;
    }
    return r;
}

static void r1375_rec(int rem, int maxp, int *parts, int *cnts, int k,
                      seq_t nfact, seq_t *sum) {
    if (rem == 0) {
        uint64_t e = 0;
        seq_t z = (seq_t)1;
        for (int x = 0; x < k; x++) {
            for (int y = 0; y < k; y++)
                e += gcd_u64((uint64_t)parts[x], (uint64_t)parts[y]) *
                     (uint64_t)cnts[x] * (uint64_t)cnts[y];
            for (int t = 1; t <= cnts[x]; t++) {
                z *= (seq_t)parts[x];
                z *= (seq_t)t;
            }
        }
        *sum += (nfact / z) * r1375_pow8(e);
        return;
    }
    for (int p = (maxp < rem ? maxp : rem); p >= 1; p--) {
        for (int c = 1; c * p <= rem; c++) {
            parts[k] = p;
            cnts[k] = c;
            r1375_rec(rem - c * p, p - 1, parts, cnts, k + 1, nfact, sum);
        }
    }
}

seq_t a(idx_t n) {
    if (n == 0) return (seq_t)0;
    int N = (int)n;
    int *parts = (int *)malloc((size_t)(N + 1) * sizeof(int));
    int *cnts = (int *)malloc((size_t)(N + 1) * sizeof(int));
    seq_t nfact = (seq_t)1;
    for (int i = 2; i <= N; i++) nfact *= (seq_t)i;
    seq_t sum = (seq_t)0;
    r1375_rec(N, N, parts, cnts, 0, nfact, &sum);
    free(parts);
    free(cnts);
    return sum / nfact;
}
