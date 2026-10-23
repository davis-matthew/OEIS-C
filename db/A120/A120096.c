/*
 * A120096  a(n) = (A046717(n))^2 (starting with n=1).
 * https://oeis.org/A120096
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 3, u16 n <= 5, u32 n <= 10, u64 n <= 20, u128 n <= 41,
 *            big any n
 * Helpers:   ensure_cache
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*********************************************************************
 * OEIS sequence A120096:  a(n) = (1 + 2*(-3)^n + 9^n) / 4
 * 
 * Recurrence:  a(1)=1, a(2)=25, a(3)=169,
 *              a(n) = 7*a(n-1) + 21*a(n-2) - 27*a(n-3)    for n ≥ 4
 *
 * This implementation memoises computed values in a static cache.
 * It uses the helper function `ensure_cache` to resize the cache as
 * required.  All arithmetic is performed in the type `seq_t`, which
 * satisfies the required properties for the test harness.
 *********************************************************************/

#include "../utils/ensure_cache.h"   /* provides ensure_cache() */

seq_t a(idx_t n)
{
    static seq_t *cache = NULL;
    static size_t cap = 0;

    if (n == 0)                     /* offset‑1, value undefined but return 0 */
        return (seq_t)0;

    /* Make sure the cache can hold up to index n */
    ensure_cache(n, &cache, &cap);

    /* Initialise the first three terms if not yet set */
    if (cache[1] == (seq_t)0)
    {
        cache[1] = (seq_t)1;   /* a(1) */
        cache[2] = (seq_t)25;  /* a(2) */
        cache[3] = (seq_t)169; /* a(3) */
    }

    if (n <= 3)
        return cache[n];

    /* Compute any missing terms up to n */
    for (idx_t i = 4; i <= n; ++i)
    {
        /* If term already computed, skip */
        if (cache[i] != (seq_t)0)
            continue;

        cache[i] =
            7 * cache[i - 1]                /* 7*a(n-1)   */
          + 21 * cache[i - 2]               /* 21*a(n-2)  */
          - 27 * cache[i - 3];              /* -27*a(n-3) */
    }

    return cache[n];
}
