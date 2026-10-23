/*
 * A021378  Decimal expansion of 1/374.
 * https://oeis.org/A021378
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, cons, easy
 * Formula:   a(n) = floor(10 * (10^n mod 374) / 374)
 * Range:     u8 n <= 1, u16 n <= 98, u32 n <= 98, u64 n <= 98, u128 n <= 98,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Equivalent to the nth decimal digit of 1/374, 0‑based index. */

#include <stdlib.h>
#include <stdint.h>

static seq_t *pow_cache = NULL;    /* Cache of 10^i mod 374 for i = 0..m. */
static size_t pow_cache_len = 0;   /* Number of cached entries. */

/* Compute a(n). */
seq_t a(idx_t n)
{
    /* Ensure we have pow_cache[0..n]. */
    if (n + 1 > pow_cache_len) {
        size_t new_len = (size_t)(n + 1);
        /* Allocate or grow the cache. */
        pow_cache = (seq_t *)realloc(pow_cache, new_len * sizeof(seq_t));
        /* Initialize cache[0] if this is the first allocation. */
        if (pow_cache_len == 0) {
            pow_cache[0] = (seq_t)1;          /* 10^0 mod 374 = 1 */
            pow_cache_len = 1;
        }
        /* Compute missing values. */
        for (size_t i = pow_cache_len; i < new_len; ++i) {
            pow_cache[i] = (pow_cache[i - 1] * (seq_t)10) % (seq_t)374;
        }
        pow_cache_len = new_len;
    }

    /* digit = floor(10 * pow_cache[n] / 374). */
    seq_t digit = (pow_cache[n] * (seq_t)10) / (seq_t)374;
    return digit;
}
