/*
 * A065723  Primes that when written in base 5, then reinterpreted in base 10,
 *          again give primes.
 * https://oeis.org/A065723
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   not stated
 * Range:     u8 n <= 6, u16 n <= 86, u32 n <= 48, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   is_prime_big
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* a(n) is the nth prime p such that the decimal interpretation of the
   base‑5 representation of p is also prime. */

#include <stdlib.h>
#include <stdbool.h>
#include "../utils/is_prime_big.h"

static seq_t *cache = NULL;
static size_t cache_cap = 0;
static idx_t cached_terms = 0;
static seq_t next_checked = 1;   /* start checking from 2 */

seq_t a(idx_t n)
{
    if (n == 0) return 0; /* offset 1; below‑offset behaviour is irrelevant */

    while (cached_terms < n)
    {
        seq_t cand = next_checked + 1;
        next_checked = cand;

        if (!is_prime_big(cand))
            continue;

        /* convert cand from base‑5 to decimal interpretation */
        seq_t val = cand;
        unsigned int digits[128];
        size_t len = 0;
        while (val > 0)
        {
            digits[len++] = (unsigned int)(val % 5);
            val /= 5;
        }
        if (len == 0) digits[len++] = 0;      /* handle cand == 0, not used */

        seq_t dec = 0;
        for (size_t i = len; i > 0; --i)
            dec = dec * 10 + digits[i - 1];

        if (!is_prime_big(dec))
            continue;

        if (cached_terms >= cache_cap)
        {
            size_t new_cap = cache_cap ? cache_cap * 2 : 16;
            seq_t *tmp_ptr = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
            if (!tmp_ptr) abort();  /* memory exhaustion */
            cache = tmp_ptr;
            cache_cap = new_cap;
        }

        cache[cached_terms++] = cand;
    }

    return cache[n - 1];
}
