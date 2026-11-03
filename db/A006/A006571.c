/*
 * A006571  Expansion of q*Product_{k>=1} (1-q^k)^2*(1-q^(11*k))^2.
 * https://oeis.org/A006571
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  sign, easy, nice, mult
 * Formula:   a(n) is multiplicative with a(11^e) = 1, a(p^e) =
 *            a(p)*a(p^(e-1)) - p*a(p^(e-2)) for p != 11, where a(p) = p -
 *            #{(x,y) in F_p^2 : y^2 + y = x^3 - x^2} (elliptic curve 11a3)
 * Range:     i8 n <= 2653, i16 n <= 10000, i32 n <= 10000, i64 n <= 10000,
 *            i128 n <= 10000, big any n
 * Validated: standard, pass, 116 terms through n = 9883 (2026-11-01)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>

/* a(p) for a prime p: p minus the number of affine points of y^2 + y = x^3 - x^2 over F_p */
static long long ap_prime(uint64_t p) {
    if (p == 11) return 1;
    long long *cnt = (long long *)calloc((size_t)p, sizeof(long long));
    for (uint64_t y = 0; y < p; y++) {
        uint64_t v = (y * y + y) % p;
        cnt[v]++;
    }
    long long N = 0;
    for (uint64_t x = 0; x < p; x++) {
        uint64_t x2 = (x * x) % p;
        uint64_t x3 = (x2 * x) % p;
        uint64_t f = (x3 + p - x2) % p;
        N += cnt[f];
    }
    free(cnt);
    return (long long)p - N;
}

static long long a_prime_power(uint64_t p, unsigned e) {
    if (p == 11) return 1;
    long long ap = ap_prime(p);
    long long prev2 = 1, prev1 = ap; /* a(p^0), a(p^1) */
    if (e == 0) return 1;
    for (unsigned k = 2; k <= e; k++) {
        long long cur = ap * prev1 - (long long)p * prev2;
        prev2 = prev1;
        prev1 = cur;
    }
    return prev1;
}

seq_t a(idx_t n) {
    if (n == 0) return 0;
    long long result = 1;
    uint64_t m = n;
    for (uint64_t p = 2; p * p <= m; p++) {
        if (m % p == 0) {
            unsigned e = 0;
            while (m % p == 0) { m /= p; e++; }
            result *= a_prime_power(p, e);
            if (result == 0) return (seq_t)0;
        }
    }
    if (m > 1) result *= a_prime_power(m, 1);
    return (seq_t)result;
}
