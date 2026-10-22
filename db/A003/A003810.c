/*
 * A003810  Order of simple Chevalley group A_5(q), q = prime power.
 * https://oeis.org/A003810
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = q^15*(q^2-1)*(q^3-1)*(q^4-1)*(q^5-1)*(q^6-1)/gcd(6,q-1),
 *            q = A000961(n+1) (n-th prime power > 1)
 * Range:     u8 none, u16 none, u32 none, u64 n <= 2, u128 n <= 9, big any n
 * Helpers:   gcd_u64
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include <stdint.h>
#include "../utils/gcd_u64.h"

static bool is_prime_power(uint64_t m) {
    uint64_t p;
    if (m < 2) return false;
    for (p = 2; p * p <= m; p++) {
        if (m % p == 0) {
            while (m % p == 0) m /= p;
            return m == 1;
        }
    }
    return true; /* m is prime */
}

static uint64_t nth_prime_power(idx_t n) {
    idx_t count = 0;
    uint64_t m = 1;
    while (count < n) {
        m++;
        if (is_prime_power(m)) count++;
    }
    return m;
}

static seq_t power(seq_t base, idx_t e) {
    seq_t result = 1;
    seq_t b = base;
    while (e > 0) {
        if (e & 1) result = result * b;
        e >>= 1;
        if (e > 0) b = b * b;
    }
    return result;
}

seq_t a(idx_t n) {
    uint64_t q = nth_prime_power(n);
    seq_t qs = (seq_t)q;
    uint64_t g = gcd_u64(6, q - 1);
    seq_t result = (power(qs, 2) - 1) / (seq_t)g; /* g divides q-1, hence q^2-1 */
    int i;
    for (i = 3; i <= 6; i++) {
        result = result * (power(qs, (idx_t)i) - 1);
    }
    result = result * power(qs, 15);
    return result;
}
