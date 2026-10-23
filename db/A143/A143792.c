/*
 * A143792  a(n) = the number of distinct prime divisors, p, of n that, when p
 *          is represented in binary, each p occurs at least once in the
 *          binary representation of n.
 * https://oeis.org/A143792
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  base, nonn
 * Formula:   a(n) = number of distinct prime divisors p of n such that
 *            binary(p) occurs as a substring in binary(n)
 * Range:     u8 n <= 10000, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   uint64_to_bin
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>
#include <string.h>
#include "../utils/uint64_to_bin.h"

static bool contains_substring(const char *big, const char *small)
{
    return strstr(big, small) != NULL;
}

seq_t a(idx_t n)
{
    /* Handle the offset: a(1) = 0 */
    if (n == 1ULL)
        return (seq_t)0;

    uint64_t value = (uint64_t)n;

    /* Generate binary representation of n */
    char bin_n[65];
    uint64_to_bin(value, bin_n);

    /* Factor n and collect distinct prime divisors */
    uint64_t primes[64];
    size_t prime_cnt = 0;
    uint64_t temp = value;

    for (uint64_t p = 2; p <= temp / p; ++p) {
        if (temp % p == 0) {
            /* add distinct prime */
            if (prime_cnt == 0 || primes[prime_cnt - 1] != p)
                primes[prime_cnt++] = p;
            while (temp % p == 0)
                temp /= p;
        }
    }
    if (temp > 1) { /* remaining prime factor */
        if (prime_cnt == 0 || primes[prime_cnt - 1] != temp)
            primes[prime_cnt++] = temp;
    }

    /* Count primes whose binary representation appears in bin_n */
    seq_t count = 0;
    char bin_p[65];
    for (size_t i = 0; i < prime_cnt; ++i) {
        uint64_to_bin(primes[i], bin_p);
        if (contains_substring(bin_n, bin_p))
            count++;
    }

    return count;
}
