/*
 * A236441  Möbius inversion of A235342.
 * https://oeis.org/A236441
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  sign
 * Formula:   a(n) = Sum_{d|n} b(d)*mu(n/d), b = A235342; equals b(p) if n =
 *            p^k (k>=1), else 0. b(p) = 1 - Sum_{q<p prime} e(p,q)*b(q),
 *            e(p,q) = exponent of q in p!
 * Range:     i8 n <= 5041, i16 n <= 5041, i32 n <= 5041, i64 n <= 5041,
 *            i128 n <= 5041, big any n
 * Validated: standard, pass, 137 terms through n = 4686 (2026-11-07)
 * Helpers:   build_primes_upto, mem
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>
#include "../utils/build_primes_upto.h"
#include "../utils/mem.h"

static seq_t A236441_expfact(uint64_t num, uint64_t prime) {
    seq_t e = 0;
    uint64_t d = prime;
    while (d <= num) {
        e += (seq_t)(num / d);
        if (d > num / prime) break;
        d *= prime;
    }
    return e;
}

seq_t a(idx_t n) {
    if (n <= 1) return (seq_t)0;

    /* find smallest prime factor p and check n is a power of p */
    uint64_t m = (uint64_t)n;
    uint64_t p = 0;
    for (uint64_t d = 2; d <= m / d; ++d) {
        if (m % d == 0) { p = d; break; }
    }
    if (p == 0) p = m;           /* n is prime */
    while (m % p == 0) m /= p;
    if (m != 1) return (seq_t)0;

    uint64_t *primes = NULL;
    size_t cnt = 0;
    build_primes_upto(p, &primes, &cnt);
    if (cnt == 0) { free(primes); return (seq_t)0; }

    seq_t *v = mem_new(seq_t, cnt);
    if (!v) { free(primes); return (seq_t)0; }

    for (size_t i = 0; i < cnt; ++i) {
        seq_t sum = 0;
        for (size_t j = 0; j < i; ++j)
            sum += A236441_expfact(primes[i], primes[j]) * v[j];
        v[i] = (seq_t)1 - sum;
    }
    seq_t res = v[cnt - 1];
    mem_free(v);
    free(primes);
    return res;
}
