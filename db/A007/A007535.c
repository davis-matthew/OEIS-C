/*
 * A007535  Smallest pseudoprime ( > n ) to base n: smallest composite number
 *          m > n such that n^(m-1)-1 is divisible by m.
 * https://oeis.org/A007535
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, nice, easy
 * Formula:   not stated
 * Range:     u8 n <= 1, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   is_prime
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include "../utils/is_prime.h"

/* Search: find smallest composite m > n such that n^(m-1) ≡ 1 (mod m) */

static uint64_t modpow(uint64_t base, uint64_t exp, uint64_t mod) {
    uint64_t result = 1;
    base %= mod;
    while (exp > 0) {
        if (exp & 1) result = ((__uint128_t)result * base) % mod;
        base = ((__uint128_t)base * base) % mod;
        exp >>= 1;
    }
    return result;
}

seq_t a(idx_t n) {
    uint64_t m = n + 1;
    
    while (1) {
        if (m > 1 && !is_prime(m)) {
            if (modpow((uint64_t)n, m - 1, m) == 1) {
                return (seq_t)m;
            }
        }
        m++;
    }
}
