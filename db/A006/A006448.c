/*
 * A006448  Number of n-element algebras with 1 binary operator and 1 constant
 *          (pointed groupoids).
 * https://oeis.org/A006448
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy, nice
 * Formula:   a(n) = Sum_{1*s_1+2*s_2+...=n} fixA[s_1, s_2,
 *            ...]/(1^s_1*s_1!*2^s_2*s_2!*...), fixA[s] = s_1 * Product_{i,
 *            j>=1} (Sum_{d|lcm(i,j)} d*s_d)^(gcd(i,j)*s_i*s_j). Computed as
 *            (Sum_s (n!/z(s)) * fixA[s]) / n! so every division is exact
 * Range:     u8 n <= 2, u16 n <= 3, u32 n <= 3, u64 n <= 5, u128 n <= 6,
 *            big any n
 * Helpers:   divisor_sum
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <string.h>
#include "../utils/divisor_sum.h"

static uint64_t gcd_u64(uint64_t a, uint64_t b) {
    while (b != 0) {
        uint64_t t = b;
        b = a % b;
        a = t;
    }
    return a;
}

static seq_t pow_seq(seq_t base, uint64_t exp) {
    seq_t result = 1;
    seq_t b = base;
    while (exp > 0) {
        if (exp & 1) result *= b;
        exp >>= 1;
        if (exp > 0) b *= b;
    }
    return result;
}

static void gen_partitions(idx_t remaining, idx_t max_part, idx_t *s, idx_t n,
                           const seq_t *nfact, seq_t *total) {
    if (remaining == 0) {
        if (s[1] == 0) return;

        seq_t fixA = (seq_t)s[1];

        for (idx_t i = 1; i <= n; i++) {
            if (s[i] == 0) continue;
            for (idx_t j = 1; j <= n; j++) {
                if (s[j] == 0) continue;
                uint64_t g = gcd_u64(i, j);
                uint64_t lcm_ij = (uint64_t)i / g * (uint64_t)j;
                seq_t base = divisor_sum(lcm_ij, s, n);
                uint64_t exp = g * (uint64_t)s[i] * (uint64_t)s[j];
                fixA *= pow_seq(base, exp);
            }
        }

        seq_t denom = 1;
        for (idx_t i = 1; i <= n; i++) {
            for (idx_t k = 1; k <= s[i]; k++) {
                denom *= (seq_t)i;
                denom *= (seq_t)k;
            }
        }

        seq_t classsize = *nfact / denom;
        *total += classsize * fixA;
        return;
    }

    for (idx_t part = 1; part <= remaining && part <= max_part; part++) {
        s[part]++;
        gen_partitions(remaining - part, part, s, n, nfact, total);
        s[part]--;
    }
}

seq_t a(idx_t n) {
    idx_t *s = (idx_t *)malloc((n + 1) * sizeof(idx_t));
    memset(s, 0, (n + 1) * sizeof(idx_t));

    seq_t nfact = 1;
    for (idx_t k = 2; k <= n; k++) nfact *= (seq_t)k;

    seq_t total = 0;
    gen_partitions(n, n, s, n, &nfact, &total);

    free(s);
    return total / nfact;
}
