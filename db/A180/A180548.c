/*
 * A180548  Primes that become a different prime under the mapping 4 <=> 7.
 * https://oeis.org/A180548
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  base, nonn
 * Formula:   not stated
 * Range:     u8 n <= 6, u16 n <= 642, u32 n <= 642, u64 n <= 2779,
 *            u128 n <= 2779, big any n
 * Helpers:   append_candidate, is_prime_big, next_prime
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* This function returns the n‑th prime p such that
   swapping each decimal digit 4↔7 in p gives a different
   prime.  The sequence is generated on demand by a
   simple primesieve using next_prime, and the found
   primes are cached for future queries. */
#include <stdbool.h>
#include "../utils/next_prime.h"
#include "../utils/is_prime_big.h"
#include "../utils/append_candidate.h"

/* Transform a 64‑bit prime by swapping digits 4 and 7. */
static seq_t
map_digits(uint64_t x)
{
    uint8_t digits[20];          /* 64‑bit numbers have <=20 decimal digits */
    int len = 0;

    /* Extract digits least‑significant first. */
    while (x > 0) {
        digits[len++] = (uint8_t)(x % 10);
        x /= 10;
    }

    /* Reconstruct number preserving order, swapping 4↔7. */
    seq_t res = 0;
    for (int i = len - 1; i >= 0; --i) {
        uint8_t d = digits[i];
        if (d == 4) d = 7;
        else if (d == 7) d = 4;
        res = res * 10 + d;
    }
    return res;
}

seq_t
a(idx_t n)
{
    /* Undefined for n < 1 by OEIS specification. */
    if (n < 1) return 0;

    /* Static cache of found terms. */
    static seq_t *cache      = NULL;
    static size_t cache_len  = 0;
    static size_t cache_cap  = 0;

    /* Candidate prime to start searching from. */
    static uint64_t next_candidate = 2;

    while (cache_len < n) {
        /* Get next prime ≥ next_candidate. */
        uint64_t prime_val = next_prime(next_candidate);
        next_candidate = prime_val + 1;

        seq_t prime_seq = (seq_t)prime_val;
        seq_t mapped    = map_digits(prime_val);

        /* Accept only if mapped value is a different prime. */
        if (mapped != prime_seq && is_prime_big(mapped))
            append_candidate(&cache, &cache_len, &cache_cap, prime_seq);
    }

    return cache[n - 1];
}
