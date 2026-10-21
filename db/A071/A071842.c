/*
 * A071842  Number of primes == 7 mod 8 <= n.
 * https://oeis.org/A071842
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = number of primes p ≤ n with p ≡ 7 (mod 8)
 * Range:     u8 n <= 105, u16 n <= 105, u32 n <= 105, u64 n <= 105,
 *            u128 n <= 105, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* This implementation builds a sieve incrementally and caches the answer. */
#include <stdlib.h>
#include <stdint.h>
#include <math.h>

static uint8_t *is_prime = NULL;   /* 1 = prime, 0 = composite */
static seq_t  *prime7_prefix = NULL;  /* cumulative count of primes ≡ 7 mod 8 */
static uint64_t sieve_limit = 0;   /* current limit of the sieve (inclusive) */

/* Extend the sieve up to at least new_limit.  Rebuilds the entire sieve. */
static void extend_sieve(idx_t new_limit)
{
    uint64_t old_limit = sieve_limit;
    sieve_limit = new_limit;
    /* Allocate/resize is_prime array */
    uint8_t *new_is_prime = NULL;
    if (is_prime == NULL) {
        new_is_prime = (uint8_t*)calloc(sieve_limit + 1, sizeof(uint8_t));
    } else {
        new_is_prime = (uint8_t*)realloc(is_prime, (sieve_limit + 1) * sizeof(uint8_t));
    }
    if (!new_is_prime) exit(1);
    is_prime = new_is_prime;

    /* Allocate/resize prefix array */
    seq_t *new_pref = NULL;
    if (prime7_prefix == NULL) {
        new_pref = (seq_t*)calloc(sieve_limit + 1, sizeof(seq_t));
    } else {
        new_pref = (seq_t*)realloc(prime7_prefix, (sieve_limit + 1) * sizeof(seq_t));
    }
    if (!new_pref) exit(1);
    prime7_prefix = new_pref;

    /* Initialise new entries */
    for (idx_t i = old_limit + 1; i <= sieve_limit; ++i) {
        if (i < 2)
            is_prime[i] = 0;
        else
            is_prime[i] = 1;
    }

    /* Sieve */
    idx_t sqrt_lim = (idx_t)sqrt((double)sieve_limit);
    for (idx_t i = 2; i <= sqrt_lim; ++i)
        if (is_prime[i]) {
            idx_t step = i;
            idx_t start = i * i;
            for (idx_t j = start; j <= sieve_limit; j += step)
                is_prime[j] = 0;
        }

    /* Build prefix counts */
    seq_t cnt = 0;
    for (idx_t i = 0; i <= sieve_limit; ++i) {
        if (i >= 2 && is_prime[i] && ((i % 8ULL) == 7U))
            ++cnt;
        prime7_prefix[i] = cnt;
    }
}

seq_t a(idx_t n)
{
    if (n < 2) return 0;
    if (n > sieve_limit) extend_sieve(n);
    return prime7_prefix[n];
}
