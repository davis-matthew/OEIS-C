/*
 * A123157  Sum of digits of the squares of prime numbers.
 * https://oeis.org/A123157
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   a(n) = digit_sum( p_n^2 ), where p_n is the n-th prime (p_1 = 2)
 * Range:     u8 n <= 6, u16 n <= 54, u32 n <= 72, u64 n <= 72, u128 n <= 72,
 *            big any n
 * Helpers:   digit_sum, sieve_first_n_primes
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include "../utils/sieve_first_n_primes.h"
#include "../utils/digit_sum.h"

/* Static cache of primes */
static uint32_t *prime_cache = NULL;
static idx_t prime_cnt = 0;

/* Ensure we have at least n primes cached */
static void ensure_primes(idx_t n)
{
    if (n <= prime_cnt)
        return;

    /* allocate a new array with the required number of primes */
    uint32_t *new_cache = NULL;
    size_t new_cnt = 0;
    sieve_first_n_primes(n, &new_cache, &new_cnt);

    /* free the old cache if it existed */
    if (prime_cache != NULL)
        free(prime_cache);

    prime_cache = new_cache;
    prime_cnt   = new_cnt;
}

seq_t a(idx_t n)
{
    if (n == 0)
        return 0;         /* undefined, return 0 for safety */

    ensure_primes(n);

    /* p_n is stored at index n-1 */
    seq_t p = (seq_t)prime_cache[n - 1];
    seq_t sq = p * p;      /* square of the prime */
    seq_t sum = digit_sum(sq);

    return sum;
}
