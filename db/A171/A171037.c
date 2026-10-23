/*
 * A171037  In the sequence of prime numbers, replace all the '7' digits with
 *          '3' and vice versa.
 * https://oeis.org/A171037
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base, easy
 * Formula:   a(n) = replace digits 3 and 7 in the n-th prime number
 * Range:     u8 n <= 50, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>

/* internal cache for prime numbers */
static uint64_t *prime_cache = NULL;   /* array of found primes */
static size_t prime_count = 0;         /* number of primes stored          */
static size_t prime_cap   = 0;         /* current capacity of the array    */

/* ensure that prime_cache contains at least n primes (n >= 1)      */
static void ensure_prime_count(idx_t n)
{
    if (prime_cap == 0) {
        prime_cap = 16;
        prime_cache = (uint64_t *)malloc(prime_cap * sizeof(uint64_t));
    }
    /* grow cache if needed */
    while (prime_count < n) {
        if (prime_cap == prime_count) {
            prime_cap <<= 1;
            prime_cache = (uint64_t *)realloc(prime_cache, prime_cap * sizeof(uint64_t));
        }
        /* start with the first candidate */
        uint64_t cand;
        if (prime_count == 0)
            cand = 2;
        else if (prime_cache[prime_count - 1] == 2)
            cand = 3;
        else
            cand = prime_cache[prime_count - 1] + 2;  /* skip even numbers */

        /* search until a prime is found */
        for (; ; cand += (cand == 2 ? 1 : 2)) {
            bool is_prime = true;
            /* trial division by known primes <= sqrt(cand) */
            for (size_t i = 0; i < prime_count; ++i) {
                uint64_t p = prime_cache[i];
                if (p * p > cand)
                    break;
                if (cand % p == 0) {
                    is_prime = false;
                    break;
                }
            }
            if (is_prime) {
                prime_cache[prime_count++] = cand;
                break;
            }
            /* next candidate */
            if (cand == 2) cand = 3;   /* handle the first even number */
        }
    }
}

/* transform a prime number: swap digits 3 ↔ 7 */
static seq_t transform_prime(uint64_t p)
{
    seq_t result = 0;
    seq_t multiplier = 1;
    while (p > 0) {
        unsigned d = (unsigned)(p % 10);
        if (d == 3)
            d = 7;
        else if (d == 7)
            d = 3;
        result += (seq_t)d * multiplier;
        multiplier *= (seq_t)10;
        p /= 10;
    }
    return result;
}

/* external function required by the OJ */
seq_t a(idx_t n)
{
    if (n < 1)
        return 0;                    /* undefined for n < 1 */

    ensure_prime_count(n);
    uint64_t prime = prime_cache[n - 1];
    return transform_prime(prime);
}
