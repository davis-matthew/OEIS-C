/*
 * A086051  Nontrivial numbers which are prime, contain no zero digits and
 *          yield another prime when their digits are sorted in ascending
 *          order.
 * https://oeis.org/A086051
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  base, nonn
 * Formula:   not stated
 * Range:     u8 n <= 8, u16 n <= 999, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   get_digits, is_prime_big
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence: nontrivial primes with no zero digits, whose
   digits sorted in ascending order form another prime.
   The search is performed by brute force; candidates are memoised
   so repeated calls are O(1). */

#include <stddef.h>
#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>
#include "../utils/get_digits.h"
#include "../utils/is_prime_big.h"

/* ------------------------------------------------------------------ */
/* Static storage for memoisation                                         */
/* ------------------------------------------------------------------ */
static seq_t     *seq_cache          = NULL;   /* array of sequence elements */
static size_t    cache_len           = 0;      /* number of elements stored */
static size_t    cache_cap           = 0;      /* allocated capacity */
static seq_t     next_candidate      = (seq_t)2;/* value to test next */

/* Utility: enlarge cache array when needed */
static void grow_cache(size_t needed)
{
    if (needed <= cache_cap) return;
    size_t new_cap = cache_cap ? cache_cap * 2 : 16;
    while (new_cap < needed) new_cap *= 2;
    seq_t *tmp = (seq_t *)realloc(seq_cache, new_cap * sizeof(seq_t));
    if (!tmp) abort();          /* out of memory – terminate */
    seq_cache = tmp;
    cache_cap = new_cap;
}

/* Checks whether a prime satisfies the OEIS condition. */
static bool qualifies_prime(seq_t p)
{
    /* fetch digits and length (digits are stored in reverse order) */
    uint8_t digits[64];
    size_t  len;
    get_digits(p, digits, &len);

    if (len <= 1) return false; /* single‑digit primes are excluded */

    /* count digits, detect zero digits */
    unsigned char cnt[10] = {0};
    for (size_t i = 0; i < len; ++i) {
        if (digits[i] == 0) return false; /* zero digit disqualifies */
        cnt[digits[i]]++;
    }

    /* build the ascending‑sorted number */
    seq_t asc = (seq_t)0;
    for (int d = 1; d <= 9; ++d)
        for (int c = 0; c < cnt[d]; ++c)
            asc = asc * (seq_t)10 + (seq_t)d;

    /* reject trivial case: already in ascending order */
    if (asc == p) return false;

    /* the ascending‑sorted number must also be prime */
    return is_prime_big(asc);
}

/* ------------------------------------------------------------------ */
/* Main sequence function                                               */
/* ------------------------------------------------------------------ */
seq_t a(idx_t n)
{
    if (n == 0)   /* offset is 1; return a safe value if called with 0 */
        return (seq_t)0;

    /* Expand cache until we have at least n terms */
    while (cache_len < (size_t)n) {
        grow_cache(cache_len + 1);

        /* Find the next prime */
        while (!is_prime_big(next_candidate))
            ++next_candidate;

        seq_t candidate = next_candidate;
        ++next_candidate;   /* advance for the next loop */

        /* If it qualifies, store it */
        if (qualifies_prime(candidate))
            seq_cache[cache_len++] = candidate;
    }

    return seq_cache[n - 1];
}
