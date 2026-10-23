/*
 * A115030  Number of distinct sums of subsets of the first n prime numbers.
 * https://oeis.org/A115030
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = n*(n+1)/2 for n ≤ 3 a(n) = sum_{i=1}^{n} prime_i – 6 for
 *            n > 3 (prime_i is the i‑th prime, starting with 2)
 * Range:     u8 n <= 13, u16 n <= 49, u32 n <= 49, u64 n <= 49, u128 n <= 49,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdbool.h>
#include <stdint.h>

/* static storage for primes and cumulative sums */
static uint32_t *primes = NULL;
static seq_t *cum     = NULL;
static size_t primes_cnt = 0;   /* number of primes currently stored */
static size_t primes_cap = 0;   /* allocated capacity */

/* ensure we have at least n primes stored */
static void ensure_primes(idx_t n)
{
    if (primes_cnt >= (size_t)n)
        return; /* already enough */

    /* allocate more space */
    size_t new_cap = primes_cap;
    if (new_cap == 0) new_cap = 4;
    while (new_cap < (size_t)n)
        new_cap <<= 1;                /* double until enough */

    uint32_t *tmp_primes = (uint32_t *)realloc(primes,
                                               new_cap * sizeof(uint32_t));
    if (!tmp_primes) abort();
    primes = tmp_primes;
    primes_cap = new_cap;

    seq_t *tmp_cum = (seq_t *)realloc(cum,
                                      new_cap * sizeof(seq_t));
    if (!tmp_cum) abort();
    cum = tmp_cum;

    /* start generating primes from the end of the current list */
    uint32_t candidate;
    if (primes_cnt == 0)
        candidate = 2;
    else
        candidate = primes[primes_cnt - 1] + 1;
    if (candidate == 2)
        candidate = 2; /* first prime */
    else if (candidate % 2 == 0)
        candidate++;   /* make it odd */

    seq_t total = 0;
    if (primes_cnt > 0)
        total = cum[primes_cnt - 1];

    while (primes_cnt < (size_t)n) {
        /* test candidate for primality */
        bool is_prime = true;
        for (size_t i = 0; i < primes_cnt; ++i) {
            uint32_t p = primes[i];
            if ((uint64_t)p * p > (uint64_t)candidate)
                break;
            if (candidate % p == 0) {
                is_prime = false;
                break;
            }
        }

        if (is_prime) {
            primes[primes_cnt] = candidate;
            total += (seq_t)candidate;
            cum[primes_cnt]   = total;
            ++primes_cnt;
        }

        /* advance to next odd number */
        if (candidate == 2)
            candidate = 3;
        else
            candidate += 2;
    }
}

/* main sequence function */
seq_t a(idx_t n)
{
    /* handle small n directly */
    if (n <= 3)
        return (seq_t)n * ((seq_t)n + 1) / 2;

    /* make sure we have the first n primes and their sums */
    ensure_primes(n);

    /* sum of first n primes */
    seq_t sum_n = cum[(size_t)n - 1];
    /* subtract the six forbidden sums */
    return sum_n - 6;
}
