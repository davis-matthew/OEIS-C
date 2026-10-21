/*
 * A228184  Numbers k such that k^2 + k + 41 is semiprime.
 * https://oeis.org/A228184
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, less
 * Formula:   numbers k such that k^2 + k + 41 is a semiprime (product of two
 *            primes, square allowed)
 * Range:     u8 n <= 19, u16 n <= 1370, u32 n <= 4760, u64 n <= 4760,
 *            u128 n <= 4760, big any n
 * Helpers:   _smallest_prime_factor, is_prime_small
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include <stdlib.h>
#include <stdint.h>

#include "../utils/_smallest_prime_factor.h"
#include "../utils/is_prime_small.h"

/* Helper: check whether a 64‑bit integer is semiprime. */
static bool is_semiprime_uint64(uint64_t m)
{
    uint64_t a = _smallest_prime_factor(m);
    if (a == m)                /* m is prime */
        return false;
    uint64_t b = m / a;
    return is_prime_small((seq_t)b);
}

/* The sequence function.  It lazily builds the list of terms the first time
 * it is needed and keeps it in a static cache so that future calls are
 * answered instantly. */
seq_t a(idx_t n)
{
    /* static state shared by all calls */
    static idx_t      tested_k = 0;   /* last k that was examined   */
    static seq_t     *cache    = NULL; /* array of found k's           */
    static size_t     cache_len = 0;   /* number of terms stored       */
    static size_t     cache_cap = 0;   /* allocated capacity           */

    /* Ensure that at least n terms are computed. */
    while (cache_len < n)
    {
        ++tested_k; /* next candidate k */

        /* Compute m = k^2 + k + 41 safely in 64‑bit arithmetic. */
        uint64_t m = (uint64_t)tested_k * tested_k + tested_k + 41;
        if (is_semiprime_uint64(m))
        {
            /* Grow the cache when necessary. */
            if (cache_len == cache_cap)
            {
                size_t new_cap = cache_cap ? cache_cap * 2 : 16;
                seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
                if (!tmp) abort();            /* memory error */
                cache    = tmp;
                cache_cap = new_cap;
            }
            cache[cache_len++] = (seq_t)tested_k;
        }
    }
    return cache[n - 1];
}
