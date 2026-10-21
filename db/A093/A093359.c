/*
 * A093359  Primes of the form 28*k + 1.
 * https://oeis.org/A093359
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) is the n-th prime congruent to 1 (mod 28)
 * Range:     u8 n <= 3, u16 n <= 536, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Algorithm: sieve to a fixed limit, then filter the primes,
   storing the first 20 000 terms.  The sieve limit is 3 000 000,
   which contains more than 10 000 such primes. */
#include <stdlib.h>

/* Static cache of the sequence.  20 000 entries cover the requested range. */
static seq_t  cached_terms[20000] = {0};
static size_t cached_cnt          = 0;
static int    initialized         = 0;

/* Initialise the cache once.  Do not allocate more than needed. */
static void init_cache(void)
{
    const size_t LIMIT = 3000000;          /* sufficient for n ≤ 10 000  */
    unsigned char *prime = (unsigned char*)malloc(LIMIT + 1);
    if (!prime) return;                    /* unlikely */

    /* Mark all numbers as prime initially. */
    for (size_t i = 0; i <= LIMIT; ++i) prime[i] = 1;
    prime[0] = prime[1] = 0;

    /* Classic Eratosthenes sieve. */
    for (size_t p = 2; p * p <= LIMIT; ++p) {
        if (prime[p]) {
            for (size_t j = p * p; j <= LIMIT; j += p)
                prime[j] = 0;
        }
    }

    /* Collect primes congruent to 1 modulo 28. */
    for (size_t i = 1; i <= LIMIT; ++i) {
        if (prime[i] && (i % 28) == 1) {
            cached_terms[cached_cnt++] = (seq_t)i;
            if (cached_cnt >= sizeof(cached_terms)/sizeof(cached_terms[0]))
                break;
        }
    }
    free(prime);
    initialized = 1;
}

/* Public sequence function.  n is 1‑based; values outside the
   cache are simply returned as 0 (any behavior is allowed for
   n < 1 or beyond the stored terms). */
seq_t a(idx_t n)
{
    if (!initialized) init_cache();
    if (n == 0 || n > cached_cnt) return 0;
    return cached_terms[n - 1];
}
