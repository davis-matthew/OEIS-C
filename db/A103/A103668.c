/*
 * A103668  Number of semiprimes between prime(n) and prime(n+1).
 * https://oeis.org/A103668
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  base, nonn
 * Formula:   a(n) = number of semiprimes between prime(n) and prime(n+1)
 * Range:     u8 n <= 105, u16 n <= 105, u32 n <= 105, u64 n <= 105,
 *            u128 n <= 105, big any n
 * Helpers:   estimate_prime_upper_bound
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>
#include <string.h>
#include "../utils/estimate_prime_upper_bound.h"

static uint64_t *primes = NULL;      /* 1‑based, primes[1] == 2 */
static idx_t prime_count = 0;        /* current number of stored primes */

/* Generate the first target_n primes (target_n ≥ 1) */
static void
ensure_primes(idx_t target_n)
{
    if (prime_count >= target_n)
        return;                     /* already have enough primes */

    /* We'll keep reallocating with a larger bound until we hit target_n */
    while (1) {
        /* Estimate an upper bound for the target_n‑th prime */
        idx_t new_limit = estimate_prime_upper_bound(target_n);
        /* For safety, expand a bit (≈ 50 % extra) to reduce reallocations */
        new_limit = new_limit * 3 / 2 + 10;

        /* Allocate arrays for the sieve */
        uint8_t *is_prime = (uint8_t *)malloc((size_t)(new_limit + 1));
        if (!is_prime) abort();

        /* Eratosthenes sieve */
        memset(is_prime, 1, (size_t)(new_limit + 1));
        is_prime[0] = is_prime[1] = 0;
        for (idx_t i = 2; (i * i) <= new_limit; ++i) {
            if (is_prime[(size_t)i]) {
                for (idx_t j = i * i; j <= new_limit; j += i)
                    is_prime[(size_t)j] = 0;
            }
        }

        /* Allocate/resize primes array */
        uint64_t *new_primes = (uint64_t *)realloc(primes, (size_t)(target_n + 1) * sizeof(uint64_t));
        if (!new_primes) abort();
        primes = new_primes;

        /* Fill primes array */
        idx_t count = 0;
        for (idx_t i = 2; i <= new_limit && count < target_n; ++i) {
            if (is_prime[(size_t)i])
                primes[++count] = (uint64_t)i;
        }

        free(is_prime);

        if (count >= target_n) {
            prime_count = count;
            break;
        }
        /* Not enough primes – try again with a larger limit */
        target_n = count + 1;          /* try to get at least one more */
    }
}

/* Count semiprimes in the half‑open interval (low, high) */
static seq_t
count_semi_in_interval(uint64_t low, uint64_t high)
{
    seq_t cnt = 0;
    for (uint64_t x = low + 1; x < high; ++x) {
        uint64_t remaining = x;
        seq_t factors = 0;
        /* Trial division using the pre‑computed primes */
        for (idx_t i = 1; i <= prime_count; ++i) {
            uint64_t p = primes[(size_t)i];
            if (p * p > remaining)
                break;
            while (remaining % p == 0) {
                ++factors;
                remaining /= p;
                if (factors > 2)
                    goto next_x;   /* stop counting factors */
            }
        }
        if (remaining > 1)
            ++factors;
        if (factors == 2)
            ++cnt;
    next_x:
        continue;
    }
    return cnt;
}

seq_t a(idx_t n)
{
    if (n < 1)
        return 0;                     /* offset is 1 */
    /* Need primes[ n ] and primes[ n+1 ] */
    ensure_primes(n + 1);
    uint64_t p_low  = primes[(size_t)n];
    uint64_t p_high = primes[(size_t)(n + 1)];
    return count_semi_in_interval(p_low, p_high);
}
