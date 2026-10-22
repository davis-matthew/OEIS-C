/*
 * A003808  Order of simple Chevalley group A_3(q) (or D_3(q)), q = prime
 *          power.
 * https://oeis.org/A003808
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = q^6 * (q^4 - 1) * (q^3 - 1) * (q^2 - 1) / gcd(4, q - 1),
 *            where q is the n-th prime power
 * Range:     u8 none, u16 n <= 1, u32 n <= 3, u64 n <= 12, u128 n <= 12,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>

static bool is_prime_power(uint64_t n) {
    if (n < 2) return false;
    if (n == 2) return true;
    if (n % 2 == 0) {
        uint64_t temp = n;
        while (temp % 2 == 0) {
            temp /= 2;
        }
        return temp == 1;
    }
    
    for (uint64_t p = 3; p * p <= n; p += 2) {
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

static uint64_t nth_prime_power(idx_t n) {
    uint64_t count = 0;
    uint64_t candidate = 2;
    
    while (true) {
        if (is_prime_power(candidate)) {
            count++;
            if (count == n) {
                return candidate;
            }
        }
        candidate++;
    }
}

static uint64_t gcd_ull(uint64_t a, uint64_t b) {
    while (b) {
        uint64_t temp = b;
        b = a % b;
        a = temp;
    }
    return a;
}

seq_t a(idx_t n) {
    uint64_t q = nth_prime_power(n);
    
    seq_t q_val = (seq_t)q;
    seq_t q2 = q_val * q_val;
    seq_t q3 = q2 * q_val;
    seq_t q4 = q2 * q2;
    seq_t q6 = q3 * q3;
    
    seq_t numerator = q6 * (q4 - 1) * (q3 - 1) * (q2 - 1);
    uint64_t gcd_val = gcd_ull(4, q - 1);
    seq_t denominator = (seq_t)gcd_val;
    
    return numerator / denominator;
}
