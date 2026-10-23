/*
 * A035249  Indices of the nonzero terms in expansion of Dirichlet series
 *          Product_p
 *          (1-(Kronecker(m,p)+1)*p^(-s)+Kronecker(m,p)*p^(-2s))^(-1) for m=
 *          -5.
 * https://oeis.org/A035249
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = n-th positive integer whose prime factorization contains
 *            no odd exponent at primes 2 or at any prime congruent to 11, 13,
 *            17, or 19 (mod 20)
 * Range:     u8 n <= 61, u16 n <= 61, u32 n <= 61, u64 n <= 61, u128 n <= 61,
 *            big any n
 * Helpers:   restricted_prime
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stddef.h>
#include <stdbool.h>
#include "../utils/restricted_prime.h"

/* maximum prime to sieve (sufficient for all candidates up to a few
   hundred thousand, which covers n <= 10000 ) */
#define SIEVE_LIMIT 100000

/* static storage for prime table */
static uint32_t primes[SIEVE_LIMIT / 10];
static size_t prime_cnt = 0;
static bool primes_computed = false;

/* one-time prime sieve */
static void init_primes(void)
{
    if (primes_computed) return;
    bool is_composite[SIEVE_LIMIT + 1];
    for (size_t i = 0; i <= SIEVE_LIMIT; ++i) is_composite[i] = false;

    for (uint32_t i = 2; i <= SIEVE_LIMIT; ++i) {
        if (!is_composite[i]) {
            primes[prime_cnt++] = i;
            if ((uint64_t)i * i <= SIEVE_LIMIT) {
                for (uint64_t j = (uint64_t)i * i; j <= SIEVE_LIMIT; j += i)
                    is_composite[(size_t)j] = true;
            }
        }
    }
    primes_computed = true;
}

/* check if number x satisfies the condition */
static bool valid_term(uint64_t x)
{
    if (x == 1) return true;          /* 1 is always valid */

    uint64_t temp = x;
    for (size_t i = 0; i < prime_cnt && (uint64_t)primes[i] * primes[i] <= temp; ++i) {
        uint32_t p = primes[i];
        if (temp % p == 0) {
            uint32_t e = 0;
            while (temp % p == 0) {
                temp /= p;
                ++e;
            }
            if ((e & 1U) && restricted_prime((uint64_t)p))
                return false;
        }
    }
    /* remaining prime factor > sqrt(temp) */
    if (temp > 1) {
        if (restricted_prime(temp)) /* exponent is 1 (odd) */
            return false;
    }
    return true;
}

/* the required OEIS sequence function */
seq_t a(idx_t n)
{
    if (n == 0) return 0;   /* undefined for n=0, return 0 as a safeguard */

    init_primes();          /* ensure primes are ready */

    seq_t term = 0;
    idx_t count = 0;
    uint64_t candidate = 1;
    while (true) {
        if (valid_term(candidate)) {
            ++count;
            if (count == n) {
                term = (seq_t)candidate;
                break;
            }
        }
        ++candidate;
    }
    return term;
}
