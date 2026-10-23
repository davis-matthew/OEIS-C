/*
 * A023509  Greatest prime divisor of prime(n) + 1.
 * https://oeis.org/A023509
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = gpf(prime(n)+1) (gpf = greatest prime divisor)
 * Range:     u8 n <= 99, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   largest_prime_factor
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>
#include "../utils/largest_prime_factor.h"

/* static storage for primes generated on demand */
static uint64_t *primes = NULL;          /* array of primes */
static size_t primes_cnt  = 0;           /* number of stored primes    */
static size_t primes_cap  = 0;           /* current array capacity */
static uint64_t next_candidate = 2;      /* next integer to test */

/* ensure that at least n primes are stored (1‑based index) */
static void
ensure_prime_n(idx_t n)
{
    while (primes_cnt < n) {
        /* trial‑division test for primality of next_candidate */
        bool isprime = true;
        for (size_t i = 0; i < primes_cnt && primes[i] * primes[i] <= next_candidate; ++i) {
            if (next_candidate % primes[i] == 0) {
                isprime = false;
                break;
            }
        }
        if (isprime) {
            /* grow array if needed */
            if (primes_cnt == primes_cap) {
                size_t new_cap = primes_cap ? primes_cap * 2 : 16;
                uint64_t *tmp = (uint64_t*)realloc(primes, new_cap * sizeof(uint64_t));
                if (!tmp) abort();
                primes = tmp;
                primes_cap = new_cap;
            }
            primes[primes_cnt++] = next_candidate;
        }
        ++next_candidate;
    }
}

/* required public function */
seq_t a(idx_t n)
{
    if (n == 0) return 0;                /* undefined offset, safe guard */
    ensure_prime_n(n);
    uint64_t p   = primes[n - 1];          /* prime(n) */
    uint64_t val = p + 1;                  /* prime(n)+1 */
    uint64_t gpf = largest_prime_factor(val); /* greatest prime divisor */
    return (seq_t)gpf;
}
