/*
 * A175278  Base-6 pandigital primes: primes having at least one of each digit
 *          0,1,2,3,4,5 when written in base 6.
 * https://oeis.org/A175278
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   not stated
 * Range:     u8 none, u16 n <= 41, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   isPrimeGeneric
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Base‑6 pandigital primes – brute‑force search:
   iterate integers >= 6^6; keep those containing all digits 0..5
   in base‑6 representation and are prime; return nth such prime. */
/* This implementation is memoised; results are cached so
   subsequent calls are fast. */

#include <stdlib.h>
#include <stdbool.h>
#include "../utils/isPrimeGeneric.h"

seq_t a(idx_t n)
{
    if (n == 0) return 0; /* undefined for n < 1 */

    /* static cache of found primes */
    static seq_t *cache = NULL;
    static size_t cache_cap = 0;
    static size_t cache_len = 0;
    static seq_t next_candidate = 46656; /* 6^6 */

    /* generate primes until we have at least n of them */
    while (cache_len < (size_t)n) {
        seq_t cand = next_candidate++;
        /* check that cand contains all digits 0..5 in base‑6 */
        unsigned mask = 0;
        seq_t tmp = cand;
        while (tmp > 0) {
            unsigned d = (unsigned)(tmp % (seq_t)6);
            mask |= 1u << d;
            tmp /= (seq_t)6;
        }
        if (mask != 0x3Fu) continue;   /* not pandigital */

        /* primality test */
        if (isPrimeGeneric(cand)) {
            if (cache_len == cache_cap) {
                size_t new_cap = (cache_cap == 0 ? 8 : cache_cap * 2);
                seq_t *p = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
                if (!p) return 0;          /* allocation failure */
                cache = p;
                cache_cap = new_cap;
            }
            cache[cache_len++] = cand;
        }
    }

    return cache[n - 1];
}
