/*
 * A002171  Glaisher's chi numbers. a(n) = chi(4*n + 1).
 * https://oeis.org/A002171
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign, easy, nice
 * Formula:   a(n) = b(4*n+1), b multiplicative with b(p^e) = b(p)*b(p^(e-1))
 *            - p*b(p^(e-2)), b(1) = 1, b(p) = p - #{(x,y) mod p : y^2 = x^3 -
 *            x}
 * Range:     i8 n <= 383, i16 n <= 10000, i32 n <= 10000, i64 n <= 10000,
 *            i128 n <= 10000, big any n
 * Validated: standard, pass, 102 terms through n = 9584 (2026-11-01)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

static uint64_t powmod_small(uint64_t b, uint64_t e, uint64_t m) {
    uint64_t r = 1 % m;
    b %= m;
    while (e > 0) {
        if (e & 1) r = (r * b) % m;
        b = (b * b) % m;
        e >>= 1;
    }
    return r;
}

static seq_t b_prime(uint64_t p) {
    uint64_t count = 0;
    uint64_t x;
    if (p % 4 == 3) return 0;
    for (x = 0; x < p; x++) {
        uint64_t r = ((x * x) % p * x % p + p - x) % p;
        if (r == 0) count++;
        else if (powmod_small(r, (p - 1) / 2, p) == 1) count += 2;
    }
    return (seq_t)(long long)p - (seq_t)(long long)count;
}

static seq_t b_prime_power(uint64_t p, int e) {
    seq_t bp, prev2, prev1;
    int i;
    if (e == 0) return 1;
    bp = b_prime(p);
    prev2 = 1;
    prev1 = bp;
    for (i = 2; i <= e; i++) {
        seq_t curr = bp * prev1 - (seq_t)(long long)p * prev2;
        prev2 = prev1;
        prev1 = curr;
    }
    return prev1;
}

seq_t a(idx_t n) {
    uint64_t m = 4 * (uint64_t)n + 1;
    uint64_t p;
    seq_t result = 1;
    for (p = 3; p * p <= m; p += 2) {
        if (m % p == 0) {
            int e = 0;
            while (m % p == 0) {
                m /= p;
                e++;
            }
            result *= b_prime_power(p, e);
        }
    }
    if (m > 1) result *= b_prime_power(m, 1);
    return result;
}
