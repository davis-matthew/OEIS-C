/*
 * A003843  Order of universal Chevalley group D_4(q), q = prime power.
 * https://oeis.org/A003843
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = D(A000961(n+1), 4) where D(q, 4) = q^12 * (q^2 - 1) *
 *            (q^4 - 1)^2 * (q^6 - 1)
 * Range:     u8 none, u16 none, u32 n <= 1, u64 n <= 3, u128 n <= 8,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>

static bool is_prime_power(uint64_t n) {
    if (n == 1) return true;
    if (n <= 1) return false;
    
    for (uint64_t p = 2; p * p <= n; p++) {
        if (n % p == 0) {
            uint64_t temp = n;
            while (temp % p == 0) temp /= p;
            return temp == 1;
        }
    }
    return true;
}

static uint64_t a000961(uint64_t n) {
    uint64_t count = 0;
    uint64_t num = 0;
    while (count < n) {
        num++;
        if (is_prime_power(num)) {
            count++;
        }
    }
    return num;
}

static seq_t power(seq_t q, idx_t e) {
    seq_t result = 1;
    for (idx_t i = 0; i < e; i++) {
        result *= q;
    }
    return result;
}

seq_t a(idx_t n) {
    uint64_t q_u64 = a000961(n + 1);
    seq_t q = (seq_t)q_u64;
    
    seq_t q2 = q * q;
    seq_t q4 = q2 * q2;
    seq_t q6 = q4 * q2;
    
    seq_t f1 = q2 - 1;
    seq_t f2 = q4 - 1;
    seq_t f3 = q6 - 1;
    
    seq_t result = power(q, 12);
    result *= f1;
    result *= f2;
    result *= f2;
    result *= f3;
    
    return result;
}
