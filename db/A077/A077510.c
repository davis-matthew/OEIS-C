/*
 * A077510  Numbers k such that k + pi(k) is a prime.
 * https://oeis.org/A077510
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) is the n-th integer k such that k+pi(k) is prime
 * Range:     u8 n <= 48, u16 n <= 6486, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* The implementation builds a sieve of primes up to a dynamic limit and
   iteratively counts numbers satisfying the condition until the n-th one
   is found. */

#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>

static size_t sieve_limit = 800000;                 /* initial sieve range */
static uint8_t *is_prime = NULL;                    /* 1 iff index is prime  */
static uint32_t *pi_cnt  = NULL;                    /* pi(x) for x <= sieve_limit */

static void build_sieve(size_t limit)
{
    free(is_prime);
    free(pi_cnt);
    is_prime = (uint8_t*)malloc(limit + 1);
    pi_cnt   = (uint32_t*)malloc((limit + 1) * sizeof(uint32_t));
    if (!is_prime || !pi_cnt) exit(EXIT_FAILURE);

    for (size_t i = 0; i <= limit; ++i) is_prime[i] = 1;
    if (limit >= 0) { is_prime[0] = 0; is_prime[1] = 0; }
    for (size_t p = 2; p * p <= limit; ++p)
        if (is_prime[p])
            for (size_t mult = p * p; mult <= limit; mult += p)
                is_prime[mult] = 0;

    pi_cnt[0] = 0;
    for (size_t i = 1; i <= limit; ++i)
        pi_cnt[i] = pi_cnt[i - 1] + is_prime[i];
    sieve_limit = limit;
}

static bool sieve_ready = false;

seq_t a(idx_t n)
{
    if (!sieve_ready) { build_sieve(sieve_limit); sieve_ready = true; }

    idx_t    k = 2;
    idx_t    found = 0;

    while (1) {
        /* enlarge sieve if needed for k or k+pi(k) */
        if (k > (idx_t)sieve_limit) {
            build_sieve(sieve_limit * 2);
        }

        uint32_t pi_k = pi_cnt[k];
        idx_t    sum  = k + pi_k;

        if (sum > (idx_t)sieve_limit) {
            build_sieve(sum * 2);   /* ensure sum is within range */
        }

        if (is_prime[sum]) {
            ++found;
            if (found == n) return (seq_t)k;
        }
        ++k;
    }
}
