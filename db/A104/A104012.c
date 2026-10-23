/*
 * A104012  Indices of centered dodecahedral numbers (A005904) which are
 *          semiprimes (A001358).
 * https://oeis.org/A104012
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = nth integer m such that both 2*m+1 and 5*m*m+5*m+1 are
 *            prime (indices of semifinished centered dodecahedral numbers –
 *            OEIS A104012)
 * Range:     u8 n <= 35, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   is_prime_uint64
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>

#include "../utils/is_prime_uint64.h"     /* bool is_prime_uint64(uint64_t) */

/* static cache for the discovered indices */
static idx_t *cache = NULL;          /* indices m */
static size_t cache_cap = 0;         /* capacity in elements */
static size_t cache_used = 0;        /* number of cached terms */

/* --------------------------------------------------------------------- */
seq_t a(idx_t n)
{
    if (n == 0) /* offset 1 – behaviour for n<1 is irrelevant */
        return 0;

    /* Grow cache capacity if needed */
    if (cache_used < n && cache_cap < n) {
        size_t new_cap = cache_cap ? cache_cap : 64;
        while (new_cap < n) new_cap <<= 1;
        idx_t *tmp = (idx_t *)realloc(cache, new_cap * sizeof(idx_t));
        if (!tmp) abort();     /* unrecoverable memory allocation failure */
        cache = tmp;
        cache_cap = new_cap;
    }

    /* Generate additional terms until we have at least n */
    if (cache_used < n) {
        idx_t start = cache_used ? cache[cache_used - 1] + 1 : 1;
        for (idx_t m = start; cache_used < n; ++m) {
            /* First factor */
            uint64_t f1 = (uint64_t)(2 * m + 1);
            if (!is_prime_uint64(f1))
                continue;

            /* Second factor, computed in 128‑bit to avoid overflow */
            unsigned __int128 prod = (unsigned __int128)5 * m * m
                                   + (unsigned __int128)5 * m + 1;
            uint64_t f2 = (uint64_t)prod;   /* safe for the needed range */
            if (!is_prime_uint64(f2))
                continue;

            /* Found a valid index */
            cache[cache_used++] = m;
        }
    }

    return (seq_t)cache[n - 1];
}
