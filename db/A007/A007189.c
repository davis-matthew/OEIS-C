/*
 * A007189  Multiplicative encoding of Stirling numbers of first kind.
 * https://oeis.org/A007189
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Product_{k=1..n} prime(k)^abs(stirling1(n, k))
 * Range:     u8 n <= 2, u16 n <= 3, u32 n <= 3, u64 n <= 4, u128 n <= 4,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>

static seq_t pow_seq(seq_t b, unsigned long long e) {
    seq_t r = 1;
    while (e) {
        if (e & 1ULL) r = r * b;
        e >>= 1;
        if (e) b = b * b;
    }
    return r;
}

static bool small_isprime(unsigned long long m) {
    if (m < 2) return false;
    if (m % 2 == 0) return m == 2;
    for (unsigned long long d = 3; d * d <= m; d += 2)
        if (m % d == 0) return false;
    return true;
}

seq_t a(idx_t n) {
    unsigned long long *s = (unsigned long long *)calloc((size_t)(n + 2), sizeof(unsigned long long));
    s[0] = 1;
    for (idx_t i = 1; i <= n; i++) {
        /* |s(i,k)| = |s(i-1,k-1)| + (i-1)*|s(i-1,k)|, update in place from high k down */
        for (idx_t k = i; k >= 1; k--) {
            s[k] = s[k - 1] + (unsigned long long)(i - 1) * s[k];
        }
        s[0] = 0;
    }

    seq_t result = 1;
    unsigned long long p = 1;
    for (idx_t k = 1; k <= n; k++) {
        p++;
        while (!small_isprime(p)) p++;
        result = result * pow_seq((seq_t)p, s[k]);
    }
    free(s);
    return result;
}
