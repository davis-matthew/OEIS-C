/*
 * A063482  a(n) = prime(n) * last digit of prime(n).
 * https://oeis.org/A063482
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  base, nonn
 * Formula:   a(n) = prime(n) * last digit of prime(n)
 * Range:     u8 n <= 9, u16 n <= 931, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdbool.h>

/* Generates the first n prime numbers on demand and returns
   prime[n-1] * (prime[n-1] % 10).  idx_t and seq_t are defined by
   the build system. */

static size_t  prime_cnt   = 0;          /* number of primes stored          */
static size_t  prime_cap   = 0;          /* capacity of the primes array     */
static uint64_t *primes    = NULL;       /* array of primes                  */
static uint64_t  candidate  = 2;         /* next integer to test for primality */

/* Ensure that at least 'target' primes are stored in 'primes'. */
static void 
ensure_prime_count(size_t target)
{
    if (prime_cnt >= target) return;

    if (!primes) {
        prime_cap = 16;
        primes = (uint64_t*)malloc(prime_cap * sizeof(uint64_t));
    }

    while (prime_cnt < target) {
        /* Find the next prime candidate */
        while (true) {
            bool isprime = true;
            if (candidate < 2) {
                candidate = 2;
                continue;
            }
            for (size_t i = 0; i < prime_cnt && primes[i] * primes[i] <= candidate; ++i) {
                if (candidate % primes[i] == 0) { isprime = false; break; }
            }
            if (isprime) {
                if (prime_cnt == prime_cap) {
                    prime_cap *= 2;
                    primes = (uint64_t*)realloc(primes, prime_cap * sizeof(uint64_t));
                }
                primes[prime_cnt++] = candidate;
                ++candidate;           /* move to next integer after a prime is found */
                break;                 /* exit inner loop and check if we have enough primes */
            }
            ++candidate;               /* not prime, try next integer */
        }
    }
}

seq_t a(idx_t n)
{
    if (n == 0) return 0;          /* offset undefined for n==0, return 0 safely */

    size_t target = (size_t)n;     /* convert to native size_t for array indexing */

    ensure_prime_count(target);

    uint64_t p = primes[target - 1];
    seq_t result = (seq_t)p * (seq_t)(p % 10);
    return result;
}
