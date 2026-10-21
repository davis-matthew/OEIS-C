/*
 * A073779  Number of 0's in base-3 representation of n-th prime.
 * https://oeis.org/A073779
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  base, easy, nonn
 * Formula:   not stated
 * Range:     u8 n <= 54, u16 n <= 6791, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   build_primes_upto
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computes the number of zeros in the base‑3 representation of the n‑th prime. */
#include <stdint.h>
#include <stdlib.h>
#include "../utils/build_primes_upto.h"

static uint64_t *prime_cache = NULL;   /* array of all primes found so far   */
static size_t prime_cache_cnt = 0;     /* number of primes stored            */

/* Ensure that prime_cache contains at least n primes. */
static void ensure_primes(idx_t n)
{
    if (prime_cache_cnt >= (size_t)n)
        return;                                   /* already enough */

    /* Rough upper bound for the n‑th prime:  n*20+100 is safe for n up to 10⁶. */
    idx_t limit = n * 20 + 100;
    uint64_t *tmp = NULL;
    size_t tmp_cnt = 0;

    build_primes_upto(limit, &tmp, &tmp_cnt);

    /* Replace old cache with the new one. */
    free(prime_cache);
    prime_cache    = tmp;
    prime_cache_cnt = tmp_cnt;
}

/* External function required by the test harness. */
seq_t a(idx_t n)
{
    if (n < 1)
        return 0;                                    /* out of range */

    ensure_primes(n);

    seq_t p = (seq_t)prime_cache[n - 1];   /* nth prime (1‑based) */
    seq_t zero_count = 0;

    while (p > 0) {
        if ((p % 3) == 0)
            ++zero_count;
        p /= 3;
    }

    return zero_count;
}
