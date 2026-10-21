/*
 * A095319  Primes in whose binary expansion the number of 1 bits is <= 3 +
 *          number of 0 bits.
 * https://oeis.org/A095319
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base, easy
 * Formula:   a(n) is the nth prime p such that 2·popcount(p) ≤ bitlen(p)+3
 *            (popcount = number of 1‑bits in binary expansion, bitlen =
 *            number of bits)
 * Range:     u8 n <= 45, u16 n <= 55, u32 n <= 55, u64 n <= 55, u128 n <= 55,
 *            big any n
 * Helpers:   bitlen_seq_t, is_prime_big
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include <stdlib.h>
#include "../utils/bitlen_seq_t.h"
#include "../utils/is_prime_big.h"

static unsigned popcount_seq_t(seq_t x)
{
    unsigned count = 0;
    while (x != (seq_t)0) {
        count += (unsigned)(x & (seq_t)1);
        x >>= 1;
    }
    return count;
}

static bool qualifies(seq_t p)
{
    unsigned bc = bitlen_seq_t(p);
    unsigned pc = popcount_seq_t(p);
    return (2 * pc <= bc + 3);
}

/* static cache for the sequence */
static seq_t *cache     = NULL;   /* array of qualifying primes */
static size_t cache_len = 0;      /* number of cached terms */
static size_t cache_cap = 0;      /* capacity of the cache array   */
static seq_t  last_checked = (seq_t)0;  /* last candidate that was examined */

/* Main sequence function */
seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)0;   /* out of range, not used */

    /* Grow the cache until it contains at least n terms */
    while (cache_len < n) {
        /* Find the next candidate integer */
        seq_t cand;
        if (last_checked < (seq_t)2) {
            cand = (seq_t)2;
        } else {
            cand = last_checked + ((last_checked == (seq_t)2) ? (seq_t)1 : (seq_t)2);
            /* Ensure an odd candidate after 2 */
            if (cand > (seq_t)2 && (cand & (seq_t)1) == (seq_t)0)
                cand += (seq_t)1;
        }

        /* Examine the candidate */
        if (is_prime_big(cand) && qualifies(cand)) {
            /* Store it */
            if (cache_len == cache_cap) {
                cache_cap = cache_cap ? cache_cap * 2 : 4;
                cache = (seq_t *)realloc(cache, cache_cap * sizeof(seq_t));
            }
            cache[cache_len++] = cand;
        }

        /* Prepare for the next iteration */
        last_checked = cand;
    }

    return cache[n - 1];
}
