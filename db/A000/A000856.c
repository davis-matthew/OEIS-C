/*
 * A000856  Number of n-input 2-output switching networks under the action of
 *          S(n) on the inputs and S(2) and complementing group C(2,2) on the
 *          outputs.
 * https://oeis.org/A000856
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   Burnside count over S(n) x (output group of order 8). For sigma
 *            in S(n), let C = number of cycles of sigma acting on {0,1}^n and
 *            E = number of those cycles of even length. Every
 *            fixed-point-free output element fixes nothing, because the
 *            all-zero input is a fixed point of sigma. So the elements fi...
 * Range:     u8 n <= 2, u16 n <= 3, u32 n <= 4, u64 n <= 5, u128 n <= 6,
 *            big any n
 * Validated: partial, pass, 7 terms through n = 7 (2026-11-01)
 * Helpers:   gcd_u64, pow2_u64
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>
#include "../utils/gcd_u64.h"
#include "../utils/pow2_u64.h"

static void a856_leaf(const int *parts, int k, seq_t fact, seq_t *T) {
    uint64_t ord = 1;
    for (int i = 0; i < k; i++) {
        uint64_t p = (uint64_t)parts[i];
        ord = ord / gcd_u64(ord, p) * p;
    }
    int nd = 0;
    for (uint64_t d = 1; d <= ord; d++) if (ord % d == 0) nd++;
    uint64_t *div = (uint64_t *)malloc((size_t)nd * sizeof(uint64_t));
    uint64_t *g = (uint64_t *)malloc((size_t)nd * sizeof(uint64_t));
    int t = 0;
    for (uint64_t d = 1; d <= ord; d++) if (ord % d == 0) div[t++] = d;
    uint64_t C = 0, E = 0;
    for (int i = 0; i < nd; i++) {
        uint64_t c = 0;
        for (int j = 0; j < k; j++) c += gcd_u64((uint64_t)parts[j], div[i]);
        uint64_t v = (uint64_t)1 << c;
        for (int j = 0; j < i; j++) if (div[i] % div[j] == 0) v -= g[j];
        g[i] = v;
        uint64_t cyc = v / div[i];
        C += cyc;
        if (div[i] % 2 == 0) E += cyc;
    }
    free(div);
    free(g);

    seq_t z = 1;
    int i = 0;
    while (i < k) {
        int j = i;
        while (j < k && parts[j] == parts[i]) j++;
        int m = j - i;
        for (int r = 0; r < m; r++) z *= (seq_t)parts[i];
        for (int r = 2; r <= m; r++) z *= (seq_t)r;
        i = j;
    }
    if (z == 0) return;
    seq_t cls = fact / z;
    *T += cls * (pow2_u64(2 * C - 3) + pow2_u64(C + E - 2));
}

static void a856_rec(int rem, int maxp, int *parts, int k, seq_t fact, seq_t *T) {
    if (rem == 0) { a856_leaf(parts, k, fact, T); return; }
    for (int p = (rem < maxp ? rem : maxp); p >= 1; p--) {
        parts[k] = p;
        a856_rec(rem - p, p, parts, k + 1, fact, T);
    }
}

seq_t a(idx_t n) {
    if (n == 0 || n > 62) return 0;
    int N = (int)n;
    seq_t fact = 1;
    for (int i = 2; i <= N; i++) fact *= (seq_t)i;
    if (fact == 0) return 0;
    int *parts = (int *)malloc((size_t)N * sizeof(int));
    seq_t T = 0;
    a856_rec(N, N, parts, 0, fact, &T);
    free(parts);
    return T / fact;
}
