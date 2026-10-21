/*
 * A072805  Primes of form 4k+3 written in base 3.
 * https://oeis.org/A072805
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  base, nonn
 * Formula:   a(n) is the decimal number obtained by writing the n‑th prime
 *            congruent to 3 (mod 4) in base 3 and then interpreting the
 *            resulting digits as a decimal integer. Example: 3 → "10" → 10, 7
 *            → "21" → 21, 11 → "102" → 102
 * Range:     u8 n <= 5, u16 n <= 28, u32 n <= 35, u64 n <= 35, u128 n <= 35,
 *            big any n
 * Helpers:   base3_value
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>   /* for size_t */
#include <stdlib.h>   /* for abort, realloc */
#include "../utils/base3_value.h"

/* The build supplies the following integer types. */
seq_t a(idx_t n);

/* --------------------------------------------------------------------- */
/* Internal state (static data shared by all calls to a())                */
static seq_t *seq_cache      = NULL;   /* cached sequence values      */
static size_t seq_cache_cnt   = 0;      /* number of cached values     */
static size_t seq_cache_cap   = 0;      /* allocated capacity          */

static uint32_t *prime_list   = NULL;   /* list of primes found so far */
static size_t prime_cnt   = 0;          /* number of stored primes     */
static size_t prime_cap   = 0;          /* allocated capacity          */
static uint32_t prime_candidate = 3;    /* next odd integer to test    */
/* --------------------------------------------------------------------- */

/* Ensure that at least `needed` sequence terms are cached. */
static void extend_cache(size_t needed)
{
    /* Grow the prime list buffer if required. */
    if (prime_cnt == prime_cap) {
        size_t new_cap = prime_cap ? prime_cap * 2 : 1024;
        uint32_t *tmp = (uint32_t *)realloc(prime_list, new_cap * sizeof(uint32_t));
        if (!tmp) abort();   /* out of memory */
        prime_list = tmp;
        prime_cap = new_cap;
    }

    /* Grow the cache buffer if required. */
    if (seq_cache_cnt == seq_cache_cap) {
        size_t new_cap = seq_cache_cap ? seq_cache_cap * 2 : 1024;
        seq_t *tmp = (seq_t *)realloc(seq_cache, new_cap * sizeof(seq_t));
        if (!tmp) abort();   /* out of memory */
        seq_cache = tmp;
        seq_cache_cap = new_cap;
    }

    /* Keep generating primes until we have enough 4k+3 primes. */
    while (seq_cache_cnt < needed) {
        /* Test primality of prime_candidate using known primes. */
        uint32_t p = prime_candidate;
        int is_prime = 1;
        for (size_t i = 0; i < prime_cnt; ++i) {
            uint32_t q = prime_list[i];
            if ((uint64_t)q * (uint64_t)q > p) break;
            if (p % q == 0) {
                is_prime = 0;
                break;
            }
        }

        if (is_prime) {
            /* Store the prime in the list. */
            prime_list[prime_cnt++] = p;

            /* If it is ≡ 3 (mod 4), compute the sequence value. */
            if ((p & 3U) == 3U) {
                seq_cache[seq_cache_cnt++] = base3_value(p);
            }
        }

        /* Next odd candidate. */
        prime_candidate += 2U;
    }
}

/* --------------------------------------------------------------------- */
/* Public sequence function                                              */
seq_t a(idx_t n)
{
    if (n < 1) abort();   /* sequence is defined only for n ≥ 1 */

    /* Generate enough terms on demand. */
    extend_cache((size_t)n);

    /* Return the requested term. (1‑based index) */
    return seq_cache[n - 1];
}
