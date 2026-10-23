/*
 * A135777  Numbers having number of divisors equal to number of digits in
 *          base 7.
 * https://oeis.org/A135777
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  base, nonn
 * Formula:   not stated
 * Range:     u8 n <= 16, u16 n <= 1250, u32 n <= 1250, u64 n <= 1250,
 *            u128 n <= 1250, big any n
 * Helpers:   digit_len_base7, divisor_count_u64
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  A135777 – Numbers having number of divisors equal to number of digits in base 7.
 *
 *  The sequence is defined by
 *     a(n) = the n‑th positive integer m (n ≥ 1) for which
 *            divisor_count_u64(m) == digit_len_base7(m).
 *
 *  The routine keeps a static cache of all terms up to the largest index
 *  requested; it expands the cache lazily on each call to a(idx_t n).
 *  The implementation is purely iterative and uses only the standard
 *  library (malloc/realloc) so it works with any seq_t/idx_t combination
 *  supported by the build system.  It is guaranteed to be correct for
 *  n ≥ 1 and to be fast enough for n ≈ 10⁴. */

#include <stdint.h>
#include <stddef.h>
#include <stdlib.h>
#include <assert.h>
#include "../utils/divisor_count_u64.h"
#include "../utils/digit_len_base7.h"

/* ----- static cache of the sequence ----- */
static seq_t   *cache_data = NULL;   /* array of computed terms            */
static size_t   cache_len  = 0;      /* number of terms stored             */
static size_t   cache_cap  = 0;      /* allocated capacity                  */
static uint64_t cache_next = 1;      /* next candidate integer to test      */

/* Ensure the cache contains at least 'need' terms. */
static void ensure_cache(size_t need)
{
    while (cache_len < need)
    {
        /* Grow the array if necessary. */
        if (cache_cap == 0)
        {
            cache_cap = 16;
            cache_data = (seq_t *)malloc(sizeof(seq_t) * cache_cap);
            assert(cache_data != NULL);
        }
        else if (cache_len == cache_cap)
        {
            size_t new_cap = cache_cap * 2;
            seq_t *tmp = (seq_t *)realloc(cache_data, new_cap * sizeof(seq_t));
            assert(tmp != NULL);
            cache_data = tmp;
            cache_cap = new_cap;
        }

        /* Test the current candidate. */
        uint64_t cand = cache_next;
        uint64_t d = divisor_count_u64(cand);
        unsigned int len = digit_len_base7(cand);

        if (d == len)
        {
            cache_data[cache_len++] = (seq_t)cand;
        }
        cache_next++;
    }
}

/* ----- public sequence function ----- */
seq_t a(idx_t n)
{
    /* n is 1‑based; ensure the cache covers this index. */
    ensure_cache((size_t)n);
    return cache_data[(size_t)(n - 1)];
}
