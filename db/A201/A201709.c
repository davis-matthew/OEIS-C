/*
 * A201709  Primes of the form 10n^2 + 1.
 * https://oeis.org/A201709
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 3, u16 n <= 22, u32 n <= 2831, u64 n <= 5000,
 *            u128 n <= 5000, big any n
 * Helpers:   is_prime_big, is_prime_small
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A201709: primes of the form 10 n^2 + 1
   The function returns the n‑th such prime (a(1) is 11).  */
#include <stddef.h>
#include <stdint.h>
#include <stdbool.h>
#include <stdlib.h>
#include <limits.h>

#include "../utils/is_prime_small.h"
#include "../utils/is_prime_big.h"

seq_t a(idx_t n)
{
    /* Persistent cache of found primes.  The cache grows lazily
       and is independent of the calling order. */
    static seq_t *cache = NULL;       /* array of primes */
    static size_t cache_len = 0;      /* number of primes stored */
    static size_t cache_cap = 0;      /* allocated capacity */
    static idx_t next_k = 1;          /* next k to test */

    /* Keep generating candidates until the cache contains at least n primes. */
    while (cache_len < n) {
        /* Compute 10*k^2 + 1 using 128‑bit arithmetic to avoid overflow in
           intermediate steps. */
        unsigned __int128 kk   = (unsigned __int128)next_k * next_k;
        unsigned __int128 cand128 = kk * (unsigned __int128)10 + (unsigned __int128)1;

        /* If the candidate does not fit into the underlying seq_t (only
           a concern for fixed 64‑bit builds), we stop – the sequence is
           defined only for values that fit. */
        if (sizeof(seq_t) == sizeof(uint64_t) &&
            cand128 > (unsigned __int128)UINT64_MAX) {
            return (seq_t)0;  /* overflow – no further terms available */
        }

        seq_t cand = (seq_t)cand128;

        /* Choose a suitable primality test: trial division for small
           types, a big‑integer routine otherwise. */
        bool prime;
        if (sizeof(seq_t) <= 8) {
            prime = is_prime_small(cand);
        } else {
            prime = is_prime_big(cand);
        }

        if (prime) {
            /* Grow the cache if needed. */
            if (cache_len == cache_cap) {
                size_t new_cap = cache_cap ? cache_cap * 2 : 16;
                seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(*tmp));
                if (!tmp) {    /* allocation failed */
                    return (seq_t)0;
                }
                cache = tmp;
                cache_cap = new_cap;
            }
            cache[cache_len++] = cand;
        }
        ++next_k;
    }

    return cache[n - 1];
}
