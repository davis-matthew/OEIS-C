/*
 * A224497  a(n) = sqrt(floor(n/2)! * Product_{k=1..n} Product_{i=1..k-1}
 *          gcd(k,i)).
 * https://oeis.org/A224497
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = sqrt(floor(n/2)! * product_{k=1..n} product_{i=1..k-1}
 *            gcd(k,i))
 * Range:     u8 n <= 8, u16 n <= 11, u32 n <= 15, u64 n <= 20, u128 n <= 23,
 *            big any n
 * Helpers:   big_pow
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>
#include "../utils/big_pow.h"

seq_t a(idx_t n)
{
    if (n <= 1) {
        return (seq_t)1;
    }
    idx_t r = n / 2;          /* floor(n/2) */

    /* ---------- sieve for primes up to n ---------- */
    size_t limit = (size_t)n;
    bool *is_prime = (bool*)malloc((limit + 1) * sizeof(bool));
    if (!is_prime) return (seq_t)1;
    for (size_t i = 0; i <= limit; ++i) is_prime[i] = true;
    is_prime[0] = is_prime[1] = false;
    for (size_t i = 2; i * i <= limit; ++i) {
        if (is_prime[i]) {
            for (size_t j = i * i; j <= limit; j += i)
                is_prime[j] = false;
        }
    }
    uint64_t *primes = (uint64_t*)malloc((limit + 1) * sizeof(uint64_t));
    if (!primes) {
        free(is_prime);
        return (seq_t)1;
    }
    size_t prime_cnt = 0;
    for (idx_t i = 2; i <= n; ++i) {
        if (is_prime[(size_t)i]) primes[prime_cnt++] = (uint64_t)i;
    }
    free(is_prime);

    /* ---------- compute a(n) via prime exponents ---------- */
    seq_t result = (seq_t)1;

    for (size_t pi = 0; pi < prime_cnt; ++pi) {
        uint64_t p = primes[pi];
        __int128 sum1 = 0;
        __int128 sum2 = 0;

        /* sum1: Σ floor(n/p^i) * (floor(n/p^i)-1) */
        uint64_t p_pow = p;
        while (p_pow <= (uint64_t)n) {
            uint64_t m = n / p_pow;
            sum1 += (__int128)m * (m - 1);
            if (p_pow > (uint64_t)n / p) break;
            p_pow *= p;
        }

        /* sum2: Σ floor(r/p^i) */
        p_pow = p;
        while (p_pow <= (uint64_t)r) {
            sum2 += r / p_pow;
            if (p_pow > (uint64_t)r / p) break;
            p_pow *= p;
        }

        __int128 exp128 = (sum1 + 2 * sum2) / 4;   /* exponent of p in a(n) */
        seq_t exp = (seq_t)exp128;
        if (exp == (seq_t)0) continue;

        seq_t term = big_pow((seq_t)p, exp);       /* p^exp */
        result *= term;
    }

    free(primes);
    return result;
}
