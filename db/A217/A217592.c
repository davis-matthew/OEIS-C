/*
 * A217592  Integers that are cut in half by cycling all the decimal digits
 *          one place to the left.
 * https://oeis.org/A217592
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  base, nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 0, u16 n <= 0, u32 n <= 0, u64 n <= 9, u128 n <= 11,
 *            big any n
 * Helpers:   ensure_cache
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* OEIS A217592: Integers that are cut in half by cycling all the decimal digits one
 * place to the left.
 *
 * Formula:
 *   a(0)            = 0
 *   a(j)            = j * 105263157894736842          (j = 1 … 9)
 *   a(n + 9)        = 10^18 * a(n) + (a(n) mod 10^18) (n > 0)
 *
 * The recurrence is applied repeatedly to generate the full sequence.
 */

#include <stddef.h>                      /* for size_t */
#include "../utils/ensure_cache.h"       /* ensure_cache */

static seq_t *cache = NULL;              /* cached values */
static size_t cached_cap = 0;            /* current capacity of cache */

seq_t a(idx_t n)
{
    /* Handle the trivial case immediately. */
    if (n == 0)
        return 0;

    /* Ensure that the cache can hold indices up to n. */
    ensure_cache(n, &cache, &cached_cap);

    /* Constants used in the recurrence. */
    const seq_t base_value = (seq_t)105263157894736842ULL;      /* 105263157894736842 */
    const seq_t pow18      = (seq_t)1000000000000000000ULL;    /* 10^18 */

    /* Initialize cache[0] in case ensure_cache left it zero. */
    cache[0] = 0;

    /* Compute the first nine terms (they never change). */
    for (idx_t i = 1; i <= 9 && i <= n; ++i)
    {
        if (cache[i] == 0)                /* value 0 cannot occur for i > 0 */
            cache[i] = (seq_t)i * base_value;
    }

    /* Use the 9‑step recurrence for any further indices. */
    for (idx_t i = 10; i <= n; ++i)
    {
        if (cache[i] == 0)
        {
            seq_t prev = cache[i - 9];
            seq_t low  = prev % pow18;          /* a(n‑9) mod 10^18 */
            cache[i] = pow18 * prev + low;      /* 10^18·a(n‑9) + low   */
        }
    }

    return cache[n];
}
