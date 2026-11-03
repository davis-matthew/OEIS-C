/*
 * A133870  Primes of the form 32*n + 1.
 * https://oeis.org/A133870
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 2, u16 n <= 411, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Validated: standard, pass, 60 terms through n = 9687 (2026-11-03)
 * Helpers:   is_prime, mem
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Primes of the form 32*n + 1
   a(n) = the n‑th prime p such that p ≡ 1 (mod 32)   (n ≥ 1)
   The function generates primes lazily and caches the results.
   It relies on the supplied primality test                                           */

#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include "../utils/is_prime.h"
#include "../utils/mem.h"

static seq_t *cached_primes = NULL;     /* cached p = 32*k+1 values */
static size_t  cached_len   = 0;        /* number of cached primes */

/* ensure at least ``need`` primes are cached */
static void generate_to(idx_t need)
{
    if (cached_len >= need)
        return;

    /* start checking from the first k that produced a known prime,
       otherwise from k = 3 (since 32*3+1 = 97 is the first term)      */
    static idx_t last_checked_k = 2;   /* k such that 32*k+1 has been tested */
    idx_t k = last_checked_k + 1;

    /* enlarge the cache array */
    size_t new_cap = cached_len ? cached_len : 64;
    while (new_cap < need)
        new_cap <<= 1;

    if (new_cap > cached_len) {
        seq_t *tmp = mem_new(seq_t, new_cap);
        if (!tmp) abort();
        if (cached_primes) {
            mem_copy(tmp, cached_primes, cached_len * sizeof(seq_t));
            mem_free(cached_primes);
        }
        cached_primes = tmp;
    }

    while (cached_len < need) {
        seq_t candidate = (seq_t)(32 * k + 1);
        if (is_prime((idx_t)candidate)) {
            cached_primes[cached_len++] = candidate;
        }
        ++k;
    }
    last_checked_k = k - 1;
}

/* public function ------------------------------------------------------- */
seq_t a(idx_t n)
{
    if (n < 1) return 0;   /* undefined for n = 0 */
    generate_to(n);
    return cached_primes[n - 1];
}
