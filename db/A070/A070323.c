/*
 * A070323  Let M_n be the n X n matrix m(i,j) = min(prime(i), prime(j)); then
 *          a(n) = det(M_n).
 * https://oeis.org/A070323
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = 2 * Π_{i=1}^{n-1} (prime(i+1) – prime(i)), a(1) = 2
 * Range:     u8 n <= 6, u16 n <= 11, u32 n <= 19, u64 n <= 26, u128 n <= 26,
 *            big any n
 * Helpers:   is_prime
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* required utilities */
#include <stdlib.h>
#include <stdbool.h>
#include <stdint.h>
#include "../utils/is_prime.h"

/* cache of primes: prime_cache[0] = 2, prime_cache[1] = 3, … */
static uint64_t *prime_cache = NULL;  /* dynamically grown array of primes */
static size_t     prime_count = 0;     /* number of primes currently stored  */
static size_t     prime_cap   = 0;     /* current allocated capacity      */

/* extend the prime cache so that it contains at least `need` primes
   (i.e. primes[0 .. need-1] are valid).                                          */
static void
ensure_primes(idx_t need)
{
    if (prime_count >= need)
        return;

    /* grow the array if necessary */
    if (prime_cap < need) {
        prime_cap = need * 2 + 10;
        prime_cache = (uint64_t*)realloc(prime_cache,
                                         prime_cap * sizeof(uint64_t));
        if (prime_cache == NULL)
            exit(1);
    }

    /* initialise the cache the first time */
    if (prime_count == 0) {
        prime_cache[0] = 2;
        prime_count    = 1;
    }

    /* find missing primes */
    uint64_t candidate = prime_cache[prime_count - 1] + 1;
    if (candidate <= 2)
        candidate = 3;                     /* avoid starting at 1 or 2 */

    while (prime_count < need) {
        /* skip even numbers after 2 */
        if (candidate > 2 && ((candidate & 1) == 0)) {
            ++candidate;
            continue;
        }

        if (is_prime(candidate))          /* primality test from utils */
            prime_cache[prime_count++] = candidate;

        ++candidate;
    }
}

/* ------------------------------------------------------------- */
/*  public function required by the judge                         */
/* ------------------------------------------------------------- */
seq_t a(idx_t n)
{
    if (n == 0)                /* outside the defined offset */
        return 0;

    if (n == 1)                /* a(1) = 2 */
        return 2;

    /* we need primes[0 .. n]  (i.e. n+1 primes) to compute gaps   */
    ensure_primes((size_t)n + 1);

    seq_t prod = 2;            /* leading factor 2 */

    for (idx_t i = 1; i < n; ++i) {
        seq_t diff = (seq_t)(prime_cache[i] - prime_cache[i - 1]);
        prod *= diff;
    }

    return prod;
}
