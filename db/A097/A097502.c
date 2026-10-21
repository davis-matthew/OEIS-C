/*
 * A097502  Least integer m such that there are at least n composite numbers
 *          between m and 2*m.
 * https://oeis.org/A097502
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = min{ m : #{x composite : m <= x <= 2m} >= n }
 * Range:     u8 n <= 72, u16 n <= 72, u32 n <= 72, u64 n <= 72, u128 n <= 72,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <string.h>
#include <stdint.h>
#include <stdbool.h>

static bool initialized = false;
static idx_t sieve_limit = 2000000;            /* upper bound for sieving */
static uint8_t *is_prime = NULL;              /* 1 = prime, 0 = composite or <2 */
static idx_t *prime_count = NULL;             /* cumulative prime counts */
static uint32_t *composite_count = NULL;      /* cumulative composite counts */

/* One‑time initialization: sieve and build prefix sums */
static void init_sieve(void)
{
    if (initialized) return;

    is_prime = (uint8_t *)malloc((sieve_limit + 1) * sizeof(uint8_t));
    prime_count = (idx_t *)malloc((sieve_limit + 1) * sizeof(idx_t));
    composite_count = (uint32_t *)malloc((sieve_limit + 1) * sizeof(uint32_t));

    if (!is_prime || !prime_count || !composite_count) {
        /* Allocation failed – should never happen in normal tests */
        exit(1);
    }

    memset(is_prime, 1, (size_t)(sieve_limit + 1));
    is_prime[0] = is_prime[1] = 0;

    for (idx_t i = 2; i * i <= sieve_limit; ++i)
        if (is_prime[i])
            for (idx_t j = i * i; j <= sieve_limit; j += i)
                is_prime[j] = 0;

    idx_t pc = 0;
    for (idx_t i = 0; i <= sieve_limit; ++i) {
        if (is_prime[i]) ++pc;
        prime_count[i] = pc;
        if (i < 4) {
            composite_count[i] = 0;
        } else {
            /* composites <= i = i - 1 (exclude 1) - primes <= i */
            composite_count[i] = (uint32_t)(i - pc - 1);
        }
    }

    initialized = true;
}

/* Main sequence function */
seq_t a(idx_t n)
{
    if (n == 0) return 0;          /* undefined for n < 1, return 0 as placeholder */

    init_sieve();

    idx_t lo = 1;
    idx_t hi = sieve_limit / 2;    /* ensure 2*hi <= sieve_limit */

    while (lo < hi) {
        idx_t mid = lo + (hi - lo) / 2;
        idx_t composites_in_interval = composite_count[2 * mid] - composite_count[mid - 1];
        if (composites_in_interval >= n)
            hi = mid;
        else
            lo = mid + 1;
    }

    return (seq_t)lo;
}
