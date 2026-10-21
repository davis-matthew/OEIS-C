/*
 * A092626  Primes with one nonprime digit.
 * https://oeis.org/A092626
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base, easy
 * Formula:   a(n) is the n‑th prime which contains exactly one decimal *
 *            digit that is not a prime digit (i.e. not in {2,3,5,7})
 * Range:     u8 n <= 19, u16 n <= 854, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   one_nonprime_digit
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>
#include <stdlib.h>
#include "../utils/one_nonprime_digit.h"

/* ---------- Dynamic container for primes -------------------------------- */
static uint64_t *prime_list = NULL;   /* array of primes found so far   */
static size_t prime_len   = 0;        /* number of stored primes        */
static size_t prime_cap   = 0;        /* allocated capacity             */

/* Ensure capacity and add a new prime to prime_list.  */
static void add_prime(uint64_t p)
{
    if (prime_len == prime_cap) {
        size_t new_cap = prime_cap ? prime_cap * 2 : 16;
        uint64_t *tmp = (uint64_t *)realloc(prime_list, new_cap * sizeof(uint64_t));
        if (!tmp)
            abort();
        prime_list = tmp;
        prime_cap  = new_cap;
    }
    prime_list[prime_len++] = p;
}

/* ---------- Cache for sequence values ---------------------------------- */
static seq_t *seq_cache = NULL;   /* array of sequence terms         */
static size_t cache_len = 0;      /* number of cached terms          */
static size_t cache_cap = 0;      /* allocated capacity              */

/* Ensure capacity and add a new term to seq_cache.  */
static void add_cache(seq_t value)
{
    if (cache_len == cache_cap) {
        size_t new_cap = cache_cap ? cache_cap * 2 : 16;
        seq_t *tmp = (seq_t *)realloc(seq_cache, new_cap * sizeof(seq_t));
        if (!tmp)
            abort();
        seq_cache = tmp;
        cache_cap = new_cap;
    }
    seq_cache[cache_len++] = value;
}

/* Highest number that has already been inspected for primality.  */
static uint64_t last_checked = 1U;

/* ----------------------------------------------------------------------- */
seq_t a(idx_t n)
{
    /* Return cached value if already computed (indices are 1‑based). */
    if ((size_t)n <= cache_len)
        return seq_cache[(size_t)n - 1U];

    /* Ensure that the first prime 2 is available for trial division. */
    if (prime_len == 0)
        add_prime(2U);

    /* Scan candidates until we have found the nth term. */
    for (uint64_t cand = last_checked + 1U; ; ++cand) {
        /* Skip even numbers greater than 2. */
        if (cand > 2U && (cand & 1U) == 0U) {
            last_checked = cand;
            continue;
        }

        /* Trial division using the prime list we have gathered. */
        bool is_prime = true;
        for (size_t i = 0; i < prime_len && prime_list[i] * prime_list[i] <= cand; ++i) {
            if (cand % prime_list[i] == 0U) {
                is_prime = false;
                break;
            }
        }

        if (is_prime) {
            /* Record the new prime. */
            add_prime(cand);

            /* Check the non‑prime digit requirement. */
            if (one_nonprime_digit(cand)) {
                seq_t val = (seq_t)cand;
                add_cache(val);

                last_checked = cand;
                /* If this is the requested term, return it. */
                if ((size_t)n == cache_len)
                    return val;
                /* Otherwise continue searching for further terms. */
            }
        }

        last_checked = cand;
    }
}
