/*
 * A007672  a(n) = A002034(n)!/n.
 * https://oeis.org/A007672
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, nice
 * Formula:   a(n) = A002034(n)!/n, where A002034(n) = smallest k such that n
 *            divides k!
 * Range:     u8 n <= 6, u16 n <= 10, u32 n <= 16, u64 n <= 22, u128 n <= 29,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>

static idx_t gcd_idx(idx_t x, idx_t y) {
    while (y != 0) {
        idx_t t = x % y;
        x = y;
        y = t;
    }
    return x;
}

/* Exponent of prime p in k! (Legendre's formula) */
static idx_t legendre(idx_t p, idx_t k) {
    idx_t e = 0;
    while (k > 0) {
        k /= p;
        e += k;
    }
    return e;
}

/* Smallest k with p^e | k! */
static idx_t min_k_for_prime_power(idx_t p, idx_t e) {
    idx_t k = p;
    while (legendre(p, k) < e) k += p;
    return k;
}

/* A002034(n): smallest k such that n divides k! */
static idx_t a002034(idx_t n) {
    idx_t best = 1;
    idx_t m = n;
    for (idx_t p = 2; p * p <= m; p++) {
        if (m % p == 0) {
            idx_t e = 0;
            while (m % p == 0) {
                m /= p;
                e++;
            }
            idx_t k = min_k_for_prime_power(p, e);
            if (k > best) best = k;
        }
    }
    if (m > 1 && m > best) best = m;
    return best;
}

seq_t a(idx_t n) {
    idx_t k = a002034(n);
    idx_t rem = n;
    seq_t result = 1;
    for (idx_t i = 2; i <= k; i++) {
        idx_t f = i;
        if (rem > 1) {
            idx_t g = gcd_idx(f, rem);
            f /= g;
            rem /= g;
        }
        result *= (seq_t)f;
    }
    return result;
}
