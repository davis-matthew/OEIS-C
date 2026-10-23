/*
 * A139290  2^(2p - 1)/4, where p is prime.
 * https://oeis.org/A139290
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 2^(2p-3), where p is the n-th prime
 * Range:     u8 n <= 3, u16 n <= 4, u32 n <= 7, u64 n <= 11, u128 n <= 14,
 *            big any n
 * Helpers:   pow2
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdbool.h>
#include <stdint.h>
#include "../utils/pow2.h"

static uint64_t *primes = NULL;
static size_t primes_cnt = 0;
static size_t primes_cap = 0;
static idx_t _prime_candidate = 0;

static void ensure_primes_initialized(void)
{
    if (primes == NULL) {
        primes_cap = 128;
        primes = (uint64_t *)malloc(primes_cap * sizeof(uint64_t));
        primes[0] = 2;
        primes_cnt = 1;
        _prime_candidate = 3;
    }
}

static uint64_t get_prime_of_idx(idx_t n)
{
    ensure_primes_initialized();
    while (primes_cnt < n) {
        if ((_prime_candidate & 1) == 0)
            ++_prime_candidate;               /* make odd */
        uint64_t cand = _prime_candidate;
        bool is_prime = true;
        for (size_t i = 0; i < primes_cnt; ++i) {
            uint64_t p = primes[i];
            if ((idx_t)p * (idx_t)p > cand)
                break;
            if (cand % p == 0) {
                is_prime = false;
                break;
            }
        }
        if (is_prime) {
            if (primes_cnt >= primes_cap) {
                primes_cap <<= 1;
                primes = (uint64_t *)realloc(primes, primes_cap * sizeof(uint64_t));
            }
            primes[primes_cnt++] = cand;
        }
        _prime_candidate += 2;
    }
    return primes[n - 1];
}

seq_t a(idx_t n)
{
    if (n < 1)
        return 0ULL;
    uint64_t p = get_prime_of_idx(n);
    idx_t exp = ((idx_t)p << 1) - 3;            /* exponent 2p-3 */
    return pow2(exp);
}
