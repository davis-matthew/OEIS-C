/*
 * A007295  Number of elements (a b, c d) in GL(2,Z) with |det| = 1, trace <=
 *          n and 0 <= a <= {b, c} <= d.
 * https://oeis.org/A007295
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Sum_{t=1..n} A007203(t). Computed by counting matrices (a
 *            b; c d) with ad - bc = s, s = +-1, 0 <= a <= b,c <= d. For fixed
 *            b and coprime a <= b, d runs over d == s*a^(-1) (mod b), d >= b,
 *            with c = (ad - s)/b. These arithmetic progressions in the trace
 *            a+d are accumulated per step b and the...
 * Range:     u8 n <= 24, u16 n <= 393, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <string.h>
#include <stdint.h>

static long long inv_mod(long long x, long long m, long long *g) {
    long long r0 = m, r1 = x % m, t0 = 0, t1 = 1;
    while (r1 != 0) {
        long long q = r0 / r1, tmp;
        tmp = r0 - q * r1; r0 = r1; r1 = tmp;
        tmp = t0 - q * t1; t0 = t1; t1 = tmp;
    }
    *g = r0;
    t0 %= m;
    if (t0 < 0) t0 += m;
    return t0;
}

static uint64_t *pref = NULL;
static long long pref_max = 0;

static void build(long long N) {
    uint64_t *cnt = (uint64_t *)calloc((size_t)(N + 1), sizeof(uint64_t));
    uint32_t *arr = (uint32_t *)malloc((size_t)(N + 1) * sizeof(uint32_t));
    long long b, a, t;
    for (b = 1; b <= N; b++) {
        long long amax = b < N - b ? b : N - b;
        int any = 0;
        if (amax < 0) break;
        memset(arr, 0, (size_t)(N + 1) * sizeof(uint32_t));
        for (a = 0; a <= amax; a++) {
            long long g, inv;
            int si;
            if (b == 1) { inv = 0; g = 1; }
            else inv = inv_mod(a, b, &g);
            if (g != 1) continue;
            for (si = 0; si < 2; si++) {
                long long s = si == 0 ? 1 : -1;
                long long r, d0;
                if (a == 0 && s == 1) continue;          /* c = -1 */
                if (a == b && s == -1) continue;         /* c = d + 1 > d */
                r = (s == 1) ? inv : (inv == 0 ? 0 : b - inv);
                d0 = b + r;
                if (a >= 1 && s == 1 && d0 == b) d0 += b; /* c < a */
                if (a + d0 <= N) { arr[a + d0]++; any = 1; }
            }
        }
        if (!any) continue;
        for (t = 0; t <= N; t++) {
            if (t >= b) arr[t] += arr[t - b];
            cnt[t] += arr[t];
        }
    }
    free(arr);
    if (pref) free(pref);
    pref = (uint64_t *)malloc((size_t)(N + 1) * sizeof(uint64_t));
    pref[0] = cnt[0];
    for (t = 1; t <= N; t++) pref[t] = pref[t - 1] + cnt[t];
    free(cnt);
    pref_max = N;
}

seq_t a(idx_t n) {
    if (pref == NULL || (long long)n > pref_max) {
        long long N = (long long)n;
        if (N < 2 * pref_max) N = 2 * pref_max;
        if (N < 64) N = 64;
        build(N);
    }
    return (seq_t)pref[n];
}
