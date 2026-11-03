/*
 * A001329  Number of nonisomorphic groupoids with n elements.
 * https://oeis.org/A001329
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, nice
 * Formula:   a(n) = Sum_{1*s_1+2*s_2+...=n}
 *            fixA[s_1,s_2,...]/(1^s_1*s_1!*2^s_2*s_2!*...), fixA[s_1,s_2,...]
 *            = Product_{i,j>=1} (Sum_{d|lcm(i,j)} d*s_d)^(gcd(i,j)*s_i*s_j).
 *            Computed exactly as a(n) = (1/n!) * Sum_s (n!/z_s) * fixA[s],
 *            where n!/z_s is an integer
 * Range:     u8 n <= 2, u16 n <= 3, u32 n <= 3, u64 n <= 5, u128 n <= 6,
 *            big any n
 * Validated: standard, pass, 26 terms through n = 27 (2026-11-01)
 * Helpers:   gcd_u64, ipow_u64
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <string.h>
#include <stdint.h>
#include "../utils/gcd_u64.h"
#include "../utils/ipow_u64.h"

static seq_t a1329_fact(idx_t n) {
    seq_t r = 1;
    for (idx_t i = 2; i <= n; i++) r *= (seq_t)i;
    return r;
}

/* (n!/z_s) * fixA[s] for the cycle type s (s[i-1] = number of i-cycles) */
static seq_t a1329_term(idx_t n, const idx_t *s, seq_t nfact) {
    seq_t z = 1;
    for (idx_t i = 1; i <= n; i++) {
        idx_t c = s[i - 1];
        for (idx_t j = 1; j <= c; j++) z *= (seq_t)i * (seq_t)j;
    }
    seq_t prod = nfact / z;
    for (idx_t i = 1; i <= n; i++) {
        if (s[i - 1] == 0) continue;
        for (idx_t j = 1; j <= n; j++) {
            if (s[j - 1] == 0) continue;
            uint64_t g = gcd_u64(i, j);
            uint64_t l = i / g * j;
            uint64_t base = 0;
            for (uint64_t d = 1; d <= l && d <= n; d++)
                if (l % d == 0) base += d * (uint64_t)s[d - 1];
            uint64_t e = g * (uint64_t)s[i - 1] * (uint64_t)s[j - 1];
            prod *= ipow_u64((seq_t)base, e);
        }
    }
    return prod;
}

static void a1329_rec(idx_t rem, idx_t maxp, idx_t *s, idx_t n, seq_t nfact, seq_t *sum) {
    if (rem == 0) {
        *sum += a1329_term(n, s, nfact);
        return;
    }
    idx_t lim = rem < maxp ? rem : maxp;
    for (idx_t p = lim; p >= 1; p--) {
        s[p - 1]++;
        a1329_rec(rem - p, p, s, n, nfact, sum);
        s[p - 1]--;
    }
}

seq_t a(idx_t n) {
    if (n == 0) return 1;
    idx_t *s = (idx_t *)calloc(n, sizeof(idx_t));
    seq_t nfact = a1329_fact(n);
    seq_t sum = 0;
    a1329_rec(n, n, s, n, nfact, &sum);
    free(s);
    return sum / nfact;
}
