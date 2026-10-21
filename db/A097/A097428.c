/*
 * A097428  Integer part of the diameters of circles with prime
 *          circumferences.
 * https://oeis.org/A097428
 *
 * Offset:    2    (a(2) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 55, u16 n <= 72, u32 n <= 72, u64 n <= 72, u128 n <= 72,
 *            big any n
 * Helpers:   sieve_primes
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* 
 * Formula: a(n) = floor( p_n / pi )  for n >= 2
 * where p_n is the (n‑2)th prime (a(2) uses the first prime 2).
 */

#include <stdlib.h>
#include <math.h>
#include "../utils/sieve_primes.h"

static idx_t *prime_cache = NULL;   /* array of primes, allocated by sieve_primes */
static size_t prime_count = 0;      /* number of primes stored */

/*  
 * Ensure that at least (need_index+1) primes are available.
 * need_index is zero‑based: 0 -> first prime.
 */
static void ensure_primes(idx_t need_index)
{
    if (prime_cache && need_index < prime_count)
        return;                     /* already have enough */

    idx_t required = need_index + 1;

    /* Estimate an upper bound for the required prime.
       For n >= 6:  p_n < n (ln n + ln ln n).  Add a safety margin. */
    idx_t limit;
    if (required < 6) {
        limit = 15;
    } else {
        double nd = (double)required;
        limit = (idx_t)(nd * (log(nd) + log(log(nd))) + 3.0);
    }

    /* Generate primes up to 'limit' */
    idx_t *new_primes = NULL;
    size_t new_count = 0;
    sieve_primes(limit, &new_primes, &new_count);

    /* Replace old cache */
    free(prime_cache);
    prime_cache = new_primes;
    prime_count = new_count;
}

/* 1e18 and floor(1e18 / pi) as 128‑bit constants */
static const unsigned __int128 DEN = (unsigned __int128)1000000000000000000ULL;
static const unsigned __int128 NUM = (unsigned __int128)318309886183790671ULL;

seq_t a(idx_t n)
{
    if (n < 2)
        return (seq_t)0;          /* value for indices below offset is undefined */

    idx_t prime_index = n - 2;    /* zero‑based prime index */
    ensure_primes(prime_index);

    seq_t p = (seq_t)prime_cache[prime_index];

    /* Compute floor(p * NUM / DEN) using 128‑bit arithmetic */
    unsigned __int128 prod = (unsigned __int128)p * NUM;
    seq_t result = (seq_t)(prod / DEN);

    return result;
}
