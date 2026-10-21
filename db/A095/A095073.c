/*
 * A095073  Primes in whose binary expansion the number of 1-bits is one more
 *          than the number of 0-bits.
 * https://oeis.org/A095073
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base, easy
 * Formula:   not stated
 * Range:     u8 n <= 7, u16 n <= 513, u32 n <= 9875, u64 n <= 9875,
 *            u128 n <= 9875, big any n
 * Helpers:   alloc_seq_array, bitlen_seq_t, free_seq_array, is_prime
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A095073: primes whose binary expansion contains one more
 * 1‑bit than 0‑bits.  a(n) is the n‑th such prime (1‑based).  This
 * routine searches for primes incrementally and caches the results
 * to answer repeated calls efficiently. */
#include <stddef.h>
#include <stdbool.h>
#include <stdint.h>
#include "../utils/is_prime.h"
#include "../utils/bitlen_seq_t.h"
#include "../utils/alloc_seq_array.h"
#include "../utils/free_seq_array.h"

static seq_t  *cache = NULL;   /* cached sequence values */
static size_t  cache_len = 0;  /* number of cached values */
static size_t  cache_cap = 0;  /* current allocation size */
static idx_t   last_checked = 1;  /* last integer examined for primality */

/* Ensure the cache can hold at least 'need' elements. */
static void
ensure_cache_capacity(size_t need)
{
    if (need <= cache_cap) return;

    size_t new_cap = cache_cap ? cache_cap * 2 : 8;
    while (new_cap < need) new_cap *= 2;

    seq_t *new_cache = alloc_seq_array(new_cap);
    for (size_t i = 0; i < cache_len; ++i) new_cache[i] = cache[i];

    if (cache != NULL) free_seq_array(cache);
    cache = new_cache;
    cache_cap = new_cap;
}

/* Find the next prime > last_checked.  This simple routine uses the
 * provided is_prime() helper and increments the candidate by one each
 * time.  For the expected input size it is sufficiently fast. */
static idx_t
next_prime_candidate(void)
{
    for (idx_t cand = ++last_checked; ; ++cand) {
        if (is_prime(cand)) { last_checked = cand; return cand; }
    }
}

/* Return the n‑th prime satisfying the binary popcount condition.
 * The function is safe for any n ≥ 1.  The implementation assumes
 * that the sequence fits in seq_t for the requested n. */
seq_t
a(idx_t n)
{
    if (n == 0) return 0;          /* out of bounds – arbitrary safe value */

    if (cache == NULL) ensure_cache_capacity(8);

    while (cache_len < (size_t)n) {
        idx_t next = next_prime_candidate();
        seq_t prime = (seq_t)next;

        /* count 1‑bits */
        idx_t ones = 0;
        seq_t tmp = prime;
        while (tmp) {
            if ((tmp & (seq_t)1) != (seq_t)0) ++ones;
            tmp >>= 1;
        }

        idx_t bits = bitlen_seq_t(prime);
        idx_t zeros = bits - ones;

        if (ones == zeros + 1) {
            ensure_cache_capacity(cache_len + 1);
            cache[cache_len++] = prime;
        }
    }

    return cache[n - 1];
}
