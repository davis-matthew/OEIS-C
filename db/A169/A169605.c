/*
 * A169605  Numbers x of the form x = 2*y - 3 = 3*z - 2 where y and z are
 *          primes.
 * https://oeis.org/A169605
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 9, u16 n <= 47, u32 n <= 47, u64 n <= 47, u128 n <= 47,
 *            big any n
 * Helpers:   build_primes_upto, is_prime_big
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A169605: numbers x such that x = 2*y - 3 = 3*z - 2 with y and z primes
   The routine scans primes y≡2 (mod 3); for each such prime it tests whether
   z = (2*y-1)/3 is prime, and stores x = 2*y-3.  The sequence values are
   cached so that arbitrary order of calls is supported.  Build limits
   are generous enough to generate >10 000 terms.  */

#include <stdint.h>
#include <stdbool.h>
#include <stdlib.h>

#include "../utils/build_primes_upto.h"      /* build_primes_upto(limit,&primes,&cnt) */
#include "../utils/is_prime_big.h"           /* is_prime_big(seq_t) */

/* ---------- configuration ---------- */
#define PRIMES_UPTO 15000000ULL           /* enough to cover n≈10 000 */

/* ---------- static storage ---------- */
static idx_t   *prime_arr = NULL;         /* list of primes up to PRIMES_UPTO */
static size_t   prime_cnt  = 0;           /* number of primes in prime_arr */

static seq_t  *cache = NULL;             /* computed sequence values */
static size_t  cache_len   = 0;           /* number of values stored */
static size_t  cache_cap   = 0;           /* current capacity of cache */

static size_t  last_prime_idx = 0;        /* next index in prime_arr to inspect */

/* ---------- helpers ---------- */
static void grow_cache(void)
{
    size_t new_cap = (cache_cap == 0) ? 64 : cache_cap * 2;
    seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
    if (!tmp) abort();  /* defensive: abort on OOM */
    cache = tmp;
    cache_cap = new_cap;
}

static void ensure_primes(void)
{
    if (!prime_arr) {
        /* build_primes_upto expects a pointer to idx_t* */
        build_primes_upto(PRIMES_UPTO, &prime_arr, &prime_cnt);
    }
}

/* Generate terms until the cache contains at least n entries.  */
static void generate_terms_up_to(idx_t n)
{
    /* n is unsigned, but cache_len is size_t.  use size_t comparison. */
    while (cache_len < (size_t)n) {
        /* search for the next suitable prime y */
        while (true) {
            if (last_prime_idx >= prime_cnt) {
                /* The prime limit was too low; abort – this should not happen
                   for n ≤ 10 000 with PRIMES_UPTO 15 million.  */
                abort();
            }
            idx_t y = prime_arr[last_prime_idx++];
            /* y must be congruent to 2 mod 3 to make z integer */
            if ((y % 3) != 2) continue;

            /* z = (2*y-1)/3  – integer because we checked the modulus   */
            seq_t z = ((seq_t)2 * (seq_t)y - 1) / 3;
            if (!is_prime_big(z)) continue;     /* z not prime */

            /* x = 2*y-3  – the sequence value                      */
            seq_t x = (seq_t)2 * (seq_t)y - 3;

            /* store in cache */
            if (cache_len == cache_cap) grow_cache();
            cache[cache_len++] = x;

            break; /* move to next term                 */
        }
    }
}

/* ---------- OEIS entry ---------- */
seq_t a(idx_t n)
{
    if (n == 0) return 0;   /* not part of the sequence (offset = 1) */

    ensure_primes();
    generate_terms_up_to(n);

    return cache[(size_t)n - 1];
}
