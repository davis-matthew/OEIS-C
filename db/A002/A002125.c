/*
 * A002125  a(n) = Sum_{k=0..n} f(k)*f(n-k) where f(k) = A002124(k).
 * https://oeis.org/A002125
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Sum_{k=0..n} f(k)*f(n-k), where f(0) = 1 and f(m) =
 *            Sum_{odd primes p <= m} f(m-p) (A002124, compositions into odd
 *            primes)
 * Range:     u8 n <= 21, u16 n <= 40, u32 n <= 80, u64 n <= 162,
 *            u128 n <= 330, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include <string.h>
#include <stdlib.h>

static seq_t *F = 0;
static idx_t Flen = 0;

static void build_f(idx_t len) {
    unsigned char *comp = (unsigned char *)malloc((size_t)len + 1);
    memset(comp, 0, (size_t)len + 1);
    for (idx_t i = 2; i * i <= len; i++) {
        if (!comp[i]) {
            for (idx_t j = i * i; j <= len; j += i) comp[j] = 1;
        }
    }
    idx_t *pr = (idx_t *)malloc(((size_t)len + 1) * sizeof(idx_t));
    idx_t np = 0;
    for (idx_t p = 3; p < len; p += 2) {
        if (!comp[p]) pr[np++] = p;
    }
    seq_t *g = (seq_t *)malloc((size_t)len * sizeof(seq_t));
    memset(g, 0, (size_t)len * sizeof(seq_t));
    g[0] = 1;
    for (idx_t m = 1; m < len; m++) {
        seq_t s = 0;
        for (idx_t j = 0; j < np && pr[j] <= m; j++) {
            s += g[m - pr[j]];
        }
        g[m] = s;
    }
    free(comp);
    free(pr);
    if (F) free(F);
    F = g;
    Flen = len;
}

seq_t a(idx_t n) {
    if (n + 1 > Flen) {
        idx_t len = n + 1;
        if (len < 2 * Flen) len = 2 * Flen;
        if (len < 64) len = 64;
        build_f(len);
    }
    seq_t result = 0;
    for (idx_t k = 0; k + k < n; k++) {
        result += F[k] * F[n - k];
    }
    result = result * 2;
    if (n % 2 == 0) result += F[n / 2] * F[n / 2];
    return result;
}
