/*
 * A045369  Primes congruent to {0, 1, 2, 4, 6} mod 7.
 * https://oeis.org/A045369
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = nth prime congruent to {0,1,2,4,6} mod 7
 * Range:     u8 n <= 35, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stddef.h>

seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)0;

    static uint32_t primes[300000];
    static size_t prime_cnt = 0;

    idx_t candidate = 2;
    idx_t found = 0;

    while (1) {
        int is_prime = 1;
        for (size_t i = 0; i < prime_cnt; ++i) {
            uint32_t p = primes[i];
            if ((uint64_t)p * (uint64_t)p > (uint64_t)candidate)
                break;
            if (candidate % p == 0) {
                is_prime = 0;
                break;
            }
        }

        if (is_prime) {
            if (prime_cnt < sizeof(primes) / sizeof(primes[0]))
                primes[prime_cnt++] = (uint32_t)candidate;
            uint32_t rem = candidate % 7U;
            if (rem != 3U && rem != 5U) {
                ++found;
                if (found == n)
                    return (seq_t)candidate;
            }
        }
        ++candidate;
    }
}
