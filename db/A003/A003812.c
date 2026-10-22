/*
 * A003812  Order of simple Chevalley group A_7(q), q = prime power.
 * https://oeis.org/A003812
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) =
 *            q^28*(q^2-1)*(q^3-1)*(q^4-1)*(q^5-1)*(q^6-1)*(q^7-1)*(q^8-1)/gcd(8,q-1),
 *            where q = A000961(n+1)
 * Range:     u8 none, u16 none, u32 none, u64 n <= 1, u128 n <= 3, big any n
 * Helpers:   a000248_pow
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>
#include "../utils/a000248_pow.h"

static uint64_t gcd_u64(uint64_t a, uint64_t b) {
    while (b) {
        uint64_t temp = b;
        b = a % b;
        a = temp;
    }
    return a;
}

static bool is_prime_power(uint64_t n) {
    if (n == 1) return true;
    if (n % 2 == 0) {
        while (n % 2 == 0) n /= 2;
        return n == 1;
    }
    for (uint64_t p = 3; p * p <= n; p += 2) {
        if (n % p == 0) {
            while (n % p == 0) n /= p;
            return n == 1;
        }
    }
    return true;
}

/* A000961(index): 1 followed by the prime powers > 1 */
static uint64_t get_prime_power(idx_t index) {
    idx_t count = 0;
    for (uint64_t m = 1; ; m++) {
        if (is_prime_power(m)) {
            count++;
            if (count == index) return m;
        }
    }
}

seq_t a(idx_t n) {
    uint64_t q = get_prime_power(n + 1);
    seq_t qs = (seq_t)q;
    uint64_t g = gcd_u64(8, q - 1);

    /* (q^2-1)/g = ((q-1)/g) * (q+1), exact division done natively */
    seq_t result = a000248_pow(qs, 28);
    result *= (seq_t)((q - 1) / g);
    result *= qs + 1;

    for (idx_t i = 3; i <= 8; i++) {
        result *= a000248_pow(qs, i) - 1;
    }
    return result;
}
