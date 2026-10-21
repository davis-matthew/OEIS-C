/*
 * A094892  a(n) is the number of primes between n*210 and (n+1)*210.
 * https://oeis.org/A094892
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = pi[(n+1)*210] - pi[n*210] where pi(x) is the
 *            prime-counting function
 * Range:     u8 n <= 0, u16 n <= 483, u32 n <= 2000, u64 n <= 2000,
 *            u128 n <= 2000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>

/* forward declaration of build_sieve: builds sieve and prefix counts up to limit */
static void build_sieve(size_t limit);

/* static storage for sieve and prefix sums */
static uint8_t  *is_prime = NULL;        /* 1 = prime, 0 = composite (index 0..n) */
static uint32_t *pi_counts = NULL;       /* pi[n]  (number of primes <= n) */
static size_t   current_limit = 0;       /* maximum index currently built */

static void build_sieve(size_t limit)
{
    /* free previous storage */
    if (is_prime)  free(is_prime);
    if (pi_counts) free(pi_counts);

    /* allocate new arrays */
    is_prime  = (uint8_t *)malloc((limit + 1) * sizeof(uint8_t));
    pi_counts = (uint32_t *)malloc((limit + 1) * sizeof(uint32_t));
    if (!is_prime || !pi_counts)
    {
        /* If memory allocation fails, terminate (the test harness will catch). */
        abort();
    }

    /* initialise: assume all numbers >=2 are prime initially */
    for (size_t i = 0; i <= limit; ++i)
        is_prime[i] = (i >= 2);

    /* Eratosthenes sieve */
    for (size_t p = 2; p * p <= limit; ++p)
    {
        if (is_prime[p])
        {
            for (size_t j = p * p; j <= limit; j += p)
                is_prime[j] = 0;
        }
    }

    /* build prefix counts */
    pi_counts[0] = 0;
    for (size_t i = 1; i <= limit; ++i)
        pi_counts[i] = pi_counts[i - 1] + is_prime[i];
    current_limit = limit;
}

/* Return a(n) as defined for OEIS sequence A094892. */
seq_t a(idx_t n)
{
    /* Convert idx_t to seq_t first to avoid overflow in the multiplication. */
    seq_t sn = n;
    seq_t low  = sn * 210;        /* lower bound partition (exclusive) */
    seq_t high = (sn + (seq_t)1) * 210;  /* upper bound (inclusive) */

    /* Ensure our sieve is built up to 'high' */
    if ((size_t)high > current_limit)
        build_sieve((size_t)high);

    /* Retrieve prime counts.  Both indices are <= current_limit. */
    uint32_t count_high = pi_counts[(size_t)high];
    uint32_t count_low  = pi_counts[(size_t)low];

    /* The difference is the number of primes in (low, high] */
    return (seq_t)(count_high - count_low);
}
