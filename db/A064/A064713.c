/*
 * A064713  Numbers k such that prime(k)^2 - k is prime.
 * https://oeis.org/A064713
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = n-th positive integer k such that prime(k)^2 - k is prime
 * Range:     u8 n <= 19, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   get_prime, is_prime_u64
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include <stdint.h>
#include "../utils/get_prime.h"
#include "../utils/is_prime_u64.h"

seq_t a(idx_t n)
{
    if (n == 0) return 0;               /* offset = 1, but n>=1 per spec */
    idx_t k = 1, count = 0;
    while (1) {
        uint64_t p = get_prime(k);       /* k-th prime, 64‑bit safe */
        __uint128_t prod = ( __uint128_t)p * p;
        uint64_t val = (uint64_t)(prod - k); /* p^2 - k, fits in 64‑bit */
        if (is_prime_u64(val)) {
            ++count;
            if (count == n)
                return (seq_t)k;         /* k fits in seq_t for all needed terms */
        }
        ++k;
    }
}
