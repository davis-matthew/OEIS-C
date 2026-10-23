/*
 * A156607  a(n) = number of odd decimal digits of n-th prime + number of
 *          prime decimal digits of n-th prime.
 * https://oeis.org/A156607
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   a(n) = (#odd decimal digits of prime(n)) + (#prime decimal
 *            digits of prime(n)) Computation: generate the first n primes
 *            once and count the required digits
 * Range:     u8 n <= 54, u16 n <= 105, u32 n <= 105, u64 n <= 105,
 *            u128 n <= 105, big any n
 * Helpers:   digit_frequencies, sieve_first_n_primes
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include <stdlib.h>
#include "../utils/sieve_first_n_primes.h"
#include "../utils/digit_frequencies.h"

static uint32_t *prime_cache = NULL;
static idx_t cache_size = 0;

static void ensure_cache(idx_t n)
{
    if (n <= cache_size) return;
    if (prime_cache) free(prime_cache);
    sieve_first_n_primes(n, &prime_cache, &cache_size);
}

seq_t a(idx_t n)
{
    if (n == (idx_t)0) return 0;          /* undefined below offset */
    ensure_cache(n);
    seq_t prime = (seq_t)prime_cache[n - 1];
    unsigned int freq[10] = {0};
    digit_frequencies(prime, freq);
    seq_t odd_cnt   = (seq_t)(freq[1] + freq[3] + freq[5] + freq[7] + freq[9]);
    seq_t prime_cnt = (seq_t)(freq[2] + freq[3] + freq[5] + freq[7]);
    return odd_cnt + prime_cnt;
}
