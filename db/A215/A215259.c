/*
 * A215259  Nearest integer to log(A003418(n)).
 * https://oeis.org/A215259
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = round(log(lcm(1..n))) = round(Sum_{p^k <= n, p prime} log
 *            p), computed with a sieve; a(0)=a(1)=0
 * Range:     u8 n <= 256, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Validated: standard, pass, 111 terms through n = 9813 (2026-11-07)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <math.h>
#include <stdint.h>

seq_t a(idx_t n) {
    if (n < 2) return (seq_t)0;
    unsigned char *comp = (unsigned char *)calloc((size_t)n + 1, 1);
    long double s = 0.0L;
    for (idx_t p = 2; p <= n; p++) {
        if (comp[p]) continue;
        if (p <= n / p) {
            for (idx_t j = p * p; j <= n; j += p) comp[j] = 1;
        }
        long double lp = logl((long double)p);
        idx_t pk = p;
        for (;;) {
            s += lp;
            if (pk > n / p) break;
            pk *= p;
        }
    }
    free(comp);
    return (seq_t)(long long)(s + 0.5L);
}
