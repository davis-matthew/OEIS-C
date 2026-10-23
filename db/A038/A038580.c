/*
 * A038580  Primes with indices that are primes with prime indices.
 * https://oeis.org/A038580
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  easy, nice, nonn
 * Formula:   a(n) = prime(prime(prime(n)))
 * Range:     u8 n <= 6, u16 n <= 146, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stddef.h>
#include <stdlib.h>
#include <stdbool.h>
#include <math.h>

static uint64_t *primes = NULL;   /* array of computed primes (1‑based values) */
static size_t    primes_cnt = 0;  /* number of primes stored */

/* Rebuild all primes up to 'limit' (inclusive).  Old data is freed. */
static void recompute_primes_upto(size_t limit)
{
    /* Allocate sieve array */
    uint8_t *is_comp = (uint8_t *)calloc(limit + 1, sizeof(uint8_t));
    if (!is_comp) abort();  /* out of memory – terminate */

    /* Classic Eratosthenes sieve */
    for (size_t i = 2; (size_t)i * i <= limit; ++i) {
        if (!is_comp[i]) {
            for (size_t j = i * i; j <= limit; j += i) {
                is_comp[j] = 1;
            }
        }
    }

    /* Count primes to allocate storage */
    size_t cnt = 0;
    for (size_t i = 2; i <= limit; ++i) {
        if (!is_comp[i]) ++cnt;
    }

    /* Allocate and fill primes array */
    uint64_t *new_primes = (uint64_t *)malloc(cnt * sizeof(uint64_t));
    if (!new_primes) abort();
    size_t idx = 0;
    for (size_t i = 2; i <= limit; ++i) {
        if (!is_comp[i]) new_primes[idx++] = i;
    }

    /* Replace old data */
    free(is_comp);
    free(primes);
    primes      = new_primes;
    primes_cnt  = cnt;
}

/* Ensure that at least 'need' primes are available in the global array. */
static void ensure_primes(idx_t need)
{
    if (primes_cnt >= (size_t)need) return;  /* already sufficient */

    /* Upper bound estimator for the n-th prime (valid for n >= 6) */
    size_t limit;
    if (need < 6) {
        limit = 15;   /* enough for the first six primes */
    } else {
        double n = (double)need;
        double l = n * (log(n) + log(log(n)));
        limit = (size_t)l + 10;  /* safety margin */
    }

    /* Build primes up to the estimated limit; grow if still insufficient */
    recompute_primes_upto(limit);
    while (primes_cnt < (size_t)need) {
        limit *= 2;                    /* double the limit */
        recompute_primes_upto(limit);
    }
}

/* Return the n-th term of the sequence a(n) = prime(prime(prime(n))) */
seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)0;     /* undefined for n < 1 */

    /* First prime: p1 = prime(n) */
    ensure_primes(n);
    uint64_t p1 = primes[(size_t)(n - 1)];

    /* Second prime: p2 = prime(p1) */
    ensure_primes(p1);
    uint64_t p2 = primes[(size_t)(p1 - 1)];

    /* Third prime: p3 = prime(p2) */
    ensure_primes(p2);
    uint64_t p3 = primes[(size_t)(p2 - 1)];

    return (seq_t)p3;
}
