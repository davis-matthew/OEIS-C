/*
 * A184793  Numbers m such that prime(m) is of the form floor(k*r), where
 *          r=(1+sqrt(5))/2; complement of A180736.
 * https://oeis.org/A184793
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   numbers m such that prime(m) is of the form floor(k*φ), where φ
 *            = (1+sqrt(5))/2. This implements A184793
 * Range:     u8 n <= 97, u16 n <= 97, u32 n <= 97, u64 n <= 97, u128 n <= 97,
 *            big any n
 * Helpers:   is_in_beatty
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>
#include <math.h>
#include <stdbool.h>
#include "../utils/is_in_beatty.h"

static seq_t *seq_cache = NULL;          /* cache of a(n) values */
static idx_t cache_len   = 0;            /* number of terms cached */
static size_t cache_cap  = 0;            /* allocated cache capacity */

static uint32_t *prime_list   = NULL;    /* list of primes found so far */
static idx_t primes_len   = 0;           /* number of primes stored */
static size_t primes_cap  = 0;           /* allocated prime list capacity */
static uint32_t next_candidate = 2;      /* next candidate for primality test */

/* Ensure prime_list contains at least `needed` primes (prime at index needed). */
static void
ensure_primes(idx_t needed)
{
    while (primes_len < needed) {
        /* find next prime */
        uint32_t cand = next_candidate++;
        if (cand < 2) continue;
        if (cand > 2 && cand % 2 == 0) continue;   /* skip even numbers >2 */

        bool isprime = true;
        uint32_t limit = (uint32_t)floor(sqrt((double)cand));
        for (idx_t i = 0; i < primes_len && prime_list[i] <= limit; ++i) {
            if (cand % prime_list[i] == 0) {
                isprime = false;
                break;
            }
        }
        if (isprime) {
            if (primes_len == primes_cap) {
                size_t new_cap = primes_cap ? primes_cap * 2 : 128;
                uint32_t *tmp = (uint32_t *)realloc(prime_list, new_cap * sizeof(uint32_t));
                if (!tmp) exit(EXIT_FAILURE);
                prime_list = tmp;
                primes_cap = new_cap;
            }
            prime_list[primes_len++] = cand;
        }
    }
}

/* Build or extend seq_cache until it contains at least n terms. */
static void
ensure_cache(idx_t n)
{
    static idx_t last_m = 0;       /* last m that has been examined */

    while (cache_len < n) {
        ++last_m;                  /* examine next integer m */
        ensure_primes(last_m);     /* make sure prime(last_m) exists */

        uint32_t p = prime_list[last_m - 1];   /* prime(m) */

        if (is_in_beatty(p)) {
            if (cache_len == cache_cap) {
                size_t new_cap = cache_cap ? cache_cap * 2 : 128;
                seq_t *tmp = (seq_t *)realloc(seq_cache, new_cap * sizeof(seq_t));
                if (!tmp) exit(EXIT_FAILURE);
                seq_cache = tmp;
                cache_cap = new_cap;
            }
            seq_cache[cache_len++] = (seq_t)last_m;
        }
    }
}

seq_t a(idx_t n)
{
    if (n == 0) return 0;               /* undefined, but safe */
    ensure_cache(n);
    return seq_cache[n - 1];
}
