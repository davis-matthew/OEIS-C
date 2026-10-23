/*
 * A115711  Squares whose digit reversal is a semiprime (A001358).
 * https://oeis.org/A115711
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   a(n) = the n‑th square whose decimal digit reversal is a
 *            semiprime
 * Range:     u8 n <= 5, u16 n <= 28, u32 n <= 41, u64 n <= 41, u128 n <= 41,
 *            big any n
 * Helpers:   is_prime, is_prime_small, reverse_decimal
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* This function generates terms lazily and caches them, so subsequent calls
   in any order are fast.  It works with all supported seq_t sizes. */

#include <stdbool.h>
#include <stdint.h>
#include <stdlib.h>
#include "../utils/reverse_decimal.h"
#include "../utils/is_prime_small.h"
#include "../utils/is_prime.h"

static bool is_semiprime_uint64(uint64_t n)
{
    if (n < 4) return false;
    for (uint64_t d = 2; d * d <= n; ++d)
    {
        if (n % d == 0)
        {
            if (!is_prime((idx_t)d)) return false;
            uint64_t q = n / d;
            return is_prime((idx_t)q);
        }
    }
    return false;
}

seq_t a(idx_t n)
{
    /* Static cache of found squares                                 */
    static seq_t *cache = NULL;
    static size_t cache_cap = 0;
    static size_t cache_size = 0;
    /* Next integer to square (root of the next candidate)           */
    static idx_t next_root = 0;

    /* One‑time cache allocation                                    */
    if (cache_cap == 0)
    {
        cache_cap = 16;
        cache = (seq_t *)malloc(cache_cap * sizeof(seq_t));
        if (!cache) abort();
    }

    /* Generate terms until we have at least n of them                */
    while (cache_size < (size_t)n)
    {
        ++next_root;                          /* root = next_root      */
        seq_t root = (seq_t)next_root;        /* cast to seq_t          */
        seq_t sq   = root * root;             /* candidate square       */

        seq_t rev = reverse_decimal(sq);      /* decimal reversal       */

        /* Only test if the reversal fits in 64‑bit; otherwise skip       */
        if (rev <= (seq_t)UINT64_MAX)
        {
            uint64_t rev64 = (uint64_t)rev;   /* safe cast in this branch   */
            if (is_semiprime_uint64(rev64))
            {
                /* enlarge cache if necessary                     */
                if (cache_size >= cache_cap)
                {
                    cache_cap *= 2;
                    seq_t *tmp = (seq_t *)realloc(cache,
                                                  cache_cap * sizeof(seq_t));
                    if (!tmp) abort();
                    cache = tmp;
                }
                cache[cache_size++] = sq;
            }
        }
    }

    return cache[n - 1];
}
