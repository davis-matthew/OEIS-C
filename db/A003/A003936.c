/*
 * A003936  Order of universal Chevalley group B_7(q), q = prime power.
 * https://oeis.org/A003936
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = q^49 * Product_{i=1..7} (q^(2i) - 1), where q =
 *            A000961(n+1) is the n-th prime power > 1
 * Range:     u8 none, u16 none, u32 none, u64 none, u128 n <= 1, big any n
 * Helpers:   a000248_pow, is_prime
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include <stdint.h>
#include "../utils/a000248_pow.h"
#include "../utils/is_prime.h"

static bool is_prime_power(uint64_t m) {
    uint64_t p;
    if (m < 2) return false;
    if (is_prime(m)) return true;
    for (p = 2; p * p <= m; p++) {
        if (m % p == 0) {
            while (m % p == 0) m /= p;
            return m == 1;
        }
    }
    return false;
}

static uint64_t nth_prime_power(idx_t n) {
    idx_t count = 1;
    uint64_t candidate = 2;
    if (n == 1) return 1;
    for (;;) {
        if (is_prime_power(candidate)) {
            count++;
            if (count == n) return candidate;
        }
        candidate++;
    }
}

seq_t a(idx_t n) {
    uint64_t q = nth_prime_power(n + 1);
    seq_t result = a000248_pow((seq_t)q, 49);
    int i;
    for (i = 1; i <= 7; i++) {
        seq_t q_pow = a000248_pow((seq_t)q, (idx_t)(2 * i));
        result = result * (q_pow - 1);
    }
    return result;
}
