/*
 * A003846  Order of universal Chevalley group D_7(q), q = prime power.
 * https://oeis.org/A003846
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = D(q,7) with q = A000961(n+1), D(q,7) =
 *            q^42*(q^7-1)*Product_{i=1..6} (q^(2i)-1)
 * Range:     u8 none, u16 none, u32 none, u64 none, u128 n <= 1, big any n
 * Helpers:   a000248_pow
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include <stdint.h>
#include "../utils/a000248_pow.h"

static bool is_prime_power_u64(uint64_t m) {
    uint64_t p;
    if (m == 1) return true;
    if (m % 2 == 0) {
        while (m % 2 == 0) m /= 2;
        return m == 1;
    }
    for (p = 3; p * p <= m; p += 2) {
        if (m % p == 0) {
            while (m % p == 0) m /= p;
            return m == 1;
        }
    }
    return true;
}

/* k-th term of A000961 (1, 2, 3, 4, 5, 7, 8, 9, ...) */
static uint64_t nth_prime_power(idx_t k) {
    idx_t count = 0;
    uint64_t m = 0;
    while (count < k) {
        m++;
        if (is_prime_power_u64(m)) count++;
    }
    return m;
}

seq_t a(idx_t n) {
    seq_t q = (seq_t)nth_prime_power(n + 1);
    seq_t result = a000248_pow(q, 42);
    int i;
    result = result * (a000248_pow(q, 7) - 1);
    for (i = 1; i <= 6; i++) {
        result = result * (a000248_pow(q, (idx_t)(2 * i)) - 1);
    }
    return result;
}
