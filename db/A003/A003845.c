/*
 * A003845  Order of universal Chevalley group D_6(q), q = prime power.
 * https://oeis.org/A003845
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = D(A000961(n+1),6) = q^30*(q^6-1)*Prod_{i=1..5}(q^(2i)-1),
 *            q = A000961(n+1)
 * Range:     u8 none, u16 none, u32 none, u64 none, u128 n <= 2, big any n
 * Helpers:   seq_pow
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include <stdint.h>
#include "../utils/seq_pow.h"

static bool is_prime_power(uint64_t x) {
    if (x < 2) return false;
    for (uint64_t p = 2; p * p <= x; p++) {
        if (x % p == 0) {
            uint64_t y = x;
            while (y % p == 0) y /= p;
            return y == 1;
        }
    }
    return true;
}

/* n-th prime power > 1, i.e. A000961(n+1) */
static uint64_t nth_prime_power(idx_t n) {
    idx_t count = 0;
    for (uint64_t x = 2;; x++) {
        if (is_prime_power(x)) {
            count++;
            if (count == n) return x;
        }
    }
}

seq_t a(idx_t n) {
    seq_t q = (seq_t)nth_prime_power(n);
    seq_t q2 = q * q;
    seq_t r = seq_pow(q, 30);
    r *= seq_pow(q, 6) - 1;
    seq_t t = 1;
    for (int i = 1; i <= 5; i++) {
        t *= q2;
        r *= t - 1;
    }
    return r;
}
