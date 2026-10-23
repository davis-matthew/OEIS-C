/*
 * A100860  Lesser of two consecutive primes of forms 4k+3 and 4k+1
 *          respectively.
 * https://oeis.org/A100860
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = the lesser of consecutive primes where the first prime ≡
 *            3 (mod 4) and the following prime ≡ 1 (mod 4)
 * Range:     u8 n <= 18, u16 n <= 54, u32 n <= 54, u64 n <= 54, u128 n <= 54,
 *            big any n
 * Helpers:   is_prime_uint64
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>
#include <stddef.h>
#include "../utils/is_prime_uint64.h"

static seq_t *a_cache = NULL;          /* array of computed terms   */
static idx_t  a_cache_len = 0;         /* number of terms stored   */
static size_t a_cache_alloc = 0;       /* allocated array size     */
static uint64_t next_prime  = 2;       /* last prime processed (prev_prime) */

/* Append a new term to the cache, extending storage if needed. */
static void cache_append(seq_t value)
{
    if (a_cache_len == a_cache_alloc) {
        size_t new_alloc = a_cache_alloc ? a_cache_alloc << 1 : 16;
        seq_t *tmp = (seq_t *)realloc(a_cache, new_alloc * sizeof(seq_t));
        if (!tmp) abort();      /* allocation failure */
        a_cache      = tmp;
        a_cache_alloc = new_alloc;
    }
    a_cache[a_cache_len++] = value;
}

/* Ensure that a_cache contains at least n terms of the sequence. */
static void ensure_cache(idx_t n)
{
    while (a_cache_len < n) {
        uint64_t curr = 0;
        /* Find the next prime after next_prime. */
        for (uint64_t cand = next_prime + 1; ; ++cand) {
            if (is_prime_uint64(cand)) { curr = cand; break; }
        }
        /* Check if pair (next_prime, curr) satisfies the condition. */
        if ((next_prime & 3ULL) == 3ULL && (curr & 3ULL) == 1ULL) {
            cache_append((seq_t)next_prime);
        }
        next_prime = curr;   /* advance to the next prime for the next iteration */
    }
}

seq_t a(idx_t n)
{
    if (n < 1) abort(); /* undefined for n < 1 */
    ensure_cache(n);
    return a_cache[n - 1];
}
