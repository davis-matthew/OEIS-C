/*
 * A085727  Number of semiprimes between n and 2n (inclusive).
 * https://oeis.org/A085727
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = number of semiprimes in the interval [n,2n] (inclusive)
 * Range:     u8 n <= 79, u16 n <= 79, u32 n <= 79, u64 n <= 79, u128 n <= 79,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>
#include <stdlib.h>

/* Simple sieve to generate primes up to 200 (enough for sqrt(20000)). */
static uint32_t primes[200];
static size_t primes_cnt = 0;

static void init_primes(void)
{
    if (primes_cnt > 0) return;
    size_t limit = 200;
    bool *sieve = (bool *)malloc(limit + 1);
    for (size_t i = 0; i <= limit; ++i) sieve[i] = true;
    sieve[0] = sieve[1] = false;
    for (size_t i = 2; i * i <= limit; ++i) {
        if (sieve[i]) {
            for (size_t j = i * i; j <= limit; j += i) sieve[j] = false;
        }
    }
    for (size_t i = 2; i <= limit; ++i) {
        if (sieve[i]) primes[primes_cnt++] = (uint32_t)i;
    }
    free(sieve);
}

/* Return the smallest prime factor of n (n >= 2).  If n is prime, returns n itself. */
static uint32_t smallest_prime_factor(uint32_t n)
{
    if (n < 2) return n;
    for (size_t i = 0; i < primes_cnt; ++i) {
        uint32_t p = primes[i];
        if ((uint64_t)p * (uint64_t)p > n) break;
        if (n % p == 0) return p;
    }
    return n;
}

seq_t a(idx_t n)
{
    if (n < 1) return 0;

    init_primes();

    idx_t low  = n;
    idx_t high = n * 2;
    seq_t count = 0;

    for (idx_t x = low; x <= high; ++x) {
        if (x < 4) continue;                 /* smallest semiprime is 2 * 2 = 4 */

        uint32_t p = smallest_prime_factor((uint32_t)x);
        if (p == x) continue;                /* x is prime, not a semiprime */

        uint32_t q = (uint32_t)x / p;         /* x = p * q  with p the smallest prime factor */
        if (smallest_prime_factor(q) == q) {  /* q is prime -> x is a semiprime */
            ++count;
        }
    }
    return count;
}
