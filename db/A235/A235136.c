/*
 * A235136  a(n) = (2*n - 1) * a(n-2) for n>1, a(0) = a(1) = 1.
 * https://oeis.org/A235136
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 6, u16 n <= 9, u32 n <= 16, u64 n <= 28, u128 n <= 48,
 *            big any n
 * Helpers:   ensure_cache
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A235136
   a(0) = a(1) = 1
   a(n) = (2*n - 1) * a(n-2)   for n > 1
   offset: 0
   (recurrence uses only integer arithmetic) */
#include <stddef.h>
#include "../utils/ensure_cache.h"

static seq_t *a_cache = NULL;
static idx_t  a_cache_len = 0;   /* number of initialized entries */
static size_t a_cache_cap = 0;   /* allocated capacity */

/* Return the nth term of A235136 */
seq_t a(idx_t n)
{
    /* Base values */
    if (n < 2) return (seq_t)1;

    /* Make sure cache can hold the required index */
    ensure_cache(n + 1, &a_cache, &a_cache_cap);

    /* Initialize first two terms once */
    if (a_cache_len == 0) {
        a_cache[0] = (seq_t)1;
        a_cache[1] = (seq_t)1;
        a_cache_len = 2;
    }

    /* Compute any missing entries up to n */
    for (idx_t i = a_cache_len; i <= n; ++i) {
        a_cache[i] = (seq_t)(2 * i - 1) * a_cache[i - 2];
    }
    a_cache_len = n + 1;

    return a_cache[n];
}
