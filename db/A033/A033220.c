/*
 * A033220  Primes of form x^2+30*y^2.
 * https://oeis.org/A033220
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) is the n-th prime that can be written as x^2 + 30*y^2
 *            (equivalently primes ≡ 1, 31, 49, or 79 (mod 120))
 * Range:     u8 n <= 5, u16 n <= 790, u32 n <= 551, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   is_prime_big
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>
#include <stdlib.h>
#include "../utils/is_prime_big.h"

static seq_t *prime_cache = NULL;
static size_t cache_cap   = 0;
static size_t cache_cnt   = 0;
static uint64_t next_candidate = 2;   /* next integer to test */

/* Ensure the cache can hold at least `min_cap` entries. */
static void grow_cache(size_t min_cap)
{
    if (min_cap <= cache_cap)
        return;
    size_t new_cap = cache_cap ? (cache_cap << 1) : 4;
    if (new_cap < min_cap)
        new_cap = min_cap;
    seq_t *tmp = (seq_t *)realloc(prime_cache, new_cap * sizeof(seq_t));
    prime_cache = tmp;
    cache_cap   = new_cap;
}

/* Return the n-th prime of the form x^2 + 30*y^2 (n >= 1). */
seq_t a(idx_t n)
{
    if (n == 0)
        return (seq_t)0;          /* undefined for n < 1, return 0 */

    /* Return cached value if already computed. */
    if ((size_t)n <= cache_cnt)
        return prime_cache[(size_t)(n - 1)];

    /* Residues modulo 120 that satisfy the form. */
    const uint8_t residues[4] = { 1, 31, 49, 79 };

    /* Find additional primes until we have at least n of them. */
    while (cache_cnt < (size_t)n) {
        uint64_t cand   = next_candidate;
        uint8_t rem     = cand % 120;

        if (rem == residues[0] || rem == residues[1] ||
            rem == residues[2] || rem == residues[3]) {

            /* Only odd numbers need testing (all residues above are odd). */
            if (is_prime_big((seq_t)cand)) {
                grow_cache(cache_cnt + 1);
                prime_cache[cache_cnt++] = (seq_t)cand;
                if (cache_cnt >= (size_t)n) {
                    /* Advance the candidate to avoid re‑testing the same number. */
                    next_candidate = cand + 1;
                    break;
                }
            }
        }
        next_candidate = cand + 1;
    }

    return prime_cache[(size_t)(n - 1)];
}
