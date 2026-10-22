/*
 * A007553  Logarithmic transform of Fibonacci numbers.
 * https://oeis.org/A007553
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   b(n) = b(n-1) - Sum_{i=1..n-2} b(i)*b(n-1-i)*binomial(n-1,i),
 *            b(0) = 1; a(n+1) = abs(b(n)). Each b(n) is stored as a magnitude
 *            in seq_t plus a separate sign flag. The sign comes from an exact
 *            comparison in big builds. In fixed-width builds it comes from a
 *            floating-point copy of b(n)/n!, and the m...
 * Range:     u8 n <= 7, u16 n <= 11, u32 n <= 16, u64 n <= 24, u128 n <= 39,
 *            big any n
 * Helpers:   grow_zero, seq_is_fixed_width
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdbool.h>
#include "../utils/grow_zero.h"
#include "../utils/seq_is_fixed_width.h"

static seq_t *mag = NULL;
static bool *ng = NULL;
static long double *cd = NULL;
static idx_t cnt = 0, cap = 0;

static void ensure(idx_t k) {
    if (k >= cap) {
        idx_t nc = cap * 2;
        if (nc < k + 1) nc = k + 1;
        if (nc < 64) nc = 64;
        mag = grow_zero(mag, cap, nc);
        ng = (bool *)realloc(ng, sizeof(bool) * nc);
        cd = (long double *)realloc(cd, sizeof(long double) * nc);
        cap = nc;
    }
    if (cnt == 0) {
        mag[0] = 1;
        ng[0] = false;
        cd[0] = 1.0L;
        cnt = 1;
    }
    bool fixed = seq_is_fixed_width();
    for (idx_t t = cnt; t <= k; t++) {
        idx_t m = t - 1;
        seq_t P = 0, N = 0;
        if (ng[m]) N += mag[m]; else P += mag[m];
        seq_t C = 1;
        long double s = 0.0L;
        for (idx_t i = 1; i < m; i++) {
            C = C * (seq_t)(m - i + 1) / (seq_t)i;
            seq_t term = mag[i] * mag[m - i] * C;
            if (ng[i] ^ ng[m - i]) P += term; else N += term;
            s += cd[i] * cd[m - i];
        }
        cd[t] = (cd[m] - s) / (long double)t;
        bool neg;
        if (fixed) neg = cd[t] < 0.0L;
        else neg = !(P >= N);
        if (neg) { mag[t] = N - P; ng[t] = true; }
        else { mag[t] = P - N; ng[t] = false; }
        cnt = t + 1;
    }
}

seq_t a(idx_t n) {
    if (n == 0) return 0;
    ensure(n - 1);
    return mag[n - 1];
}
