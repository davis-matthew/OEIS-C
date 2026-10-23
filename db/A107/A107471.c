/*
 * A107471  a(n) = 3*prime(n) - 2*prime(n+1).
 * https://oeis.org/A107471
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  sign
 * Formula:   a(n) = 3*prime(n) - 2*prime(n+1)
 * Range:     i8 n <= 32, i16 n <= 71, i32 n <= 71, i64 n <= 71, i128 n <= 71,
 *            big any n
 * Helpers:   upper_bound_prime
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include "../utils/upper_bound_prime.h"

/* local storage for prime numbers */
static uint64_t *prime_tbl = NULL;   /* dynamic array of primes */
static size_t prime_cnt = 0;         /* number of primes stored */

/* generate all primes up to 'limit' and store them in prime_tbl */
static void sieve_primes_up_to(uint64_t limit)
{
    /* allocate a bitmap for composite numbers */
    uint8_t *is_comp = (uint8_t *)calloc(limit + 1, 1);
    if (!is_comp)
        return; /* allocation failure; caller should handle it */

    for (uint64_t p = 2; p <= limit / p; ++p) {
        if (!is_comp[p]) {
            for (uint64_t m = p * p; m <= limit; m += p)
                is_comp[m] = 1;
        }
    }

    /* free old table if any */
    free(prime_tbl);
    prime_tbl = NULL;
    prime_cnt = 0;

    /* approximate capacity: limit/log(limit) < limit/10 for limit>= 60 */
    size_t capacity = limit / 10 + 10;
    prime_tbl = (uint64_t *)malloc(capacity * sizeof(uint64_t));

    for (uint64_t i = 2; i <= limit; ++i) {
        if (!is_comp[i]) {
            if (prime_cnt >= capacity) {
                capacity *= 2;
                prime_tbl = (uint64_t *)realloc(prime_tbl, capacity * sizeof(uint64_t));
            }
            prime_tbl[prime_cnt++] = i;
        }
    }

    free(is_comp);
}

/* ensure that at least 'need' primes are available (indexing from 1) */
static void ensure_primes(size_t need)
{
    if (prime_cnt >= need)
        return;

    uint64_t limit = upper_bound_prime(need);
    sieve_primes_up_to(limit);
}

/* public sequence function */
seq_t a(idx_t n)
{
    /* n is 1‑based index */
    if (n < 1)
        return 0;  /* undefined for n < 1 */

    /* we need prime(n) and prime(n+1) -> at least n+1 primes */
    ensure_primes((size_t)n + 1);

    /* safety check, though ensure_primes should guarantee it */
    if (prime_cnt < (size_t)n + 1)
        return 0;

    uint64_t p1 = prime_tbl[(size_t)n - 1];
    uint64_t p2 = prime_tbl[(size_t)n];

    /* compute 3*p1 - 2*p2 using seq_t arithmetic */
    return (seq_t)3 * (seq_t)p1 - (seq_t)2 * (seq_t)p2;
}
