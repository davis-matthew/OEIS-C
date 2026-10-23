/*
 * A129753  Floor(prime(n)/nonprime(n)).
 * https://oeis.org/A129753
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = floor(prime(n)/nonprime(n))
 * Range:     u8 n <= 50, u16 n <= 50, u32 n <= 50, u64 n <= 50, u128 n <= 50,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>

/* helper: test primality using already known primes */
static bool is_prime_candidate(uint64_t x, const uint64_t *primes, size_t primes_cnt) {
    if (x < 2) return false;
    for (size_t i = 0; i < primes_cnt; ++i) {
        uint64_t p = primes[i];
        if (p * p > x) break;
        if (x % p == 0) return false;
    }
    return true;
}

seq_t a(idx_t n) {
    static uint64_t *primes    = NULL;
    static size_t    primes_cap = 0;
    static size_t    primes_cnt = 0;

    static uint64_t *nonprimes = NULL;
    static size_t    nonprimes_cap = 0;
    static size_t    nonprimes_cnt = 0;

    static uint64_t checked = 1;  /* next number to examine */

    /* grow an array if needed */
    if (primes == NULL) {
        primes_cap = 16;
        primes = (uint64_t *)malloc(primes_cap * sizeof(uint64_t));
    }
    if (nonprimes == NULL) {
        nonprimes_cap = 16;
        nonprimes = (uint64_t *)malloc(nonprimes_cap * sizeof(uint64_t));
    }

    /* ensure we have at least n primes and n nonprimes */
    while (primes_cnt < (size_t)n || nonprimes_cnt < (size_t)n) {
        uint64_t x = checked;
        bool is_prime = is_prime_candidate(x, primes, primes_cnt);

        if (is_prime) {
            if (primes_cnt == primes_cap) {
                primes_cap *= 2;
                primes = (uint64_t *)realloc(primes, primes_cap * sizeof(uint64_t));
            }
            primes[primes_cnt++] = x;
        } else {
            if (nonprimes_cnt == nonprimes_cap) {
                nonprimes_cap *= 2;
                nonprimes = (uint64_t *)realloc(nonprimes, nonprimes_cap * sizeof(uint64_t));
            }
            nonprimes[nonprimes_cnt++] = x;
        }
        checked++;
    }

    uint64_t prime_val = primes[n - 1];
    uint64_t nonprime_val = nonprimes[n - 1];
    seq_t result = (seq_t)(prime_val / nonprime_val);
    return result;
}
