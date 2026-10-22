/*
 * A003848  Order of (usually) simple Chevalley group D_2(q), q = prime power.
 * https://oeis.org/A003848
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = q^2 * (q^2 - 1)^2 / gcd(2, q - 1)^2, where q =
 *            A000961(n+1)
 * Range:     u8 n <= 1, u16 n <= 4, u32 n <= 19, u64 n <= 27, u128 n <= 27,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>

static uint64_t gcd(uint64_t a, uint64_t b) {
    while (b) {
        uint64_t t = b;
        b = a % b;
        a = t;
    }
    return a;
}

static bool is_prime_power(uint64_t n) {
    if (n == 1) return true;
    
    for (uint64_t p = 2; p * p <= n; p++) {
        if (n % p == 0) {
            uint64_t temp = n;
            while (temp % p == 0) {
                temp /= p;
            }
            return temp == 1;
        }
    }
    return true;
}

static uint64_t a000961(idx_t n) {
    if (n == 1) return 1;
    
    uint64_t count = 1;
    for (uint64_t num = 2; ; num++) {
        if (is_prime_power(num)) {
            count++;
            if (count == n) return num;
        }
    }
}

seq_t a(idx_t n) {
    uint64_t q = a000961(n + 1);
    
    seq_t q_s = (seq_t)q;
    seq_t q2 = q_s * q_s;
    seq_t q2_minus_1 = q2 - 1;
    seq_t numerator = q2 * q2_minus_1 * q2_minus_1;
    
    uint64_t g = gcd(2, q - 1);
    seq_t denom = (seq_t)(g * g);
    
    return numerator / denom;
}
