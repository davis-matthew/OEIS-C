/*
 * A054906  Smallest number x such that sigma(x+2n) = sigma(x)+2n (first
 *          definition).
 * https://oeis.org/A054906
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = the smallest prime p such that p+2n is also prime
 * Range:     u8 n <= 262, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   build_primes_upto, is_prime_big, is_prime_small
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* The function uses a precomputed list of primes up to a fixed limit.
   For each call it scans this list until it finds a p with p+2n prime.
   If the limit is insufficient, it continues by testing successive
   odd numbers beyond the sieve limit.  */

#include <stddef.h>
#include <stdbool.h>
#include <stdint.h>

#include "../utils/build_primes_upto.h"
#include "../utils/is_prime_big.h"
#include "../utils/is_prime_small.h"

static const idx_t PRIME_SIEVE_LIMIT = 2000000; /* sufficient for moderate n */
static bool primes_built = false;
static uint64_t *primes = NULL;
static size_t prime_count = 0;

/* Helper to ensure the prime table is built once. */
static void ensure_primes(void)
{
    if (!primes_built) {
        build_primes_upto(PRIME_SIEVE_LIMIT, &primes, &prime_count);
        primes_built = true;
    }
}

seq_t a(idx_t n)
{
    if (n == 0u) return (seq_t)0;  /* undefined for offset 1, but safe return */

    ensure_primes();

    /* Compute 2n as a seq_t value.  */
    seq_t t = ((seq_t)n) * 2;

    /* Scan primes from the pre‑computed list.  */
    for (size_t i = 0; i < prime_count; ++i) {
        uint64_t p = primes[i];
        seq_t sum = t + (seq_t)p;
        if (is_prime_big(sum)) {
            return (seq_t)p;
        }
    }

    /* In the unlikely event that the table is exhausted,
       continue by testing odd candidates beyond the table.  */
    uint64_t candidate = primes[prime_count - 1] + 2;
    for (;;) {
        if (is_prime_small((seq_t)candidate)) {
            seq_t sum = t + (seq_t)candidate;
            if (is_prime_big(sum)) {
                return (seq_t)candidate;
            }
        }
        candidate += 2;
    }
}
