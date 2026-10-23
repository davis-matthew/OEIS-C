/*
 * A166297  Number of UUDUDD's starting at level 0 in all Dyck paths of
 *          semilength n with no UUU's and no DDD's (U=(1,1), D=(1,-1)).
 * https://oeis.org/A166297
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 6, u16 n <= 12, u32 n <= 24, u64 n <= 31, u128 n <= 31,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* OEIS A166297: Number of UUDUDD's starting at level 0 in all Dyck paths of
   semilength n with no UUU's and no DDD's.
   Recurrence (valid for n ≥ 6):
        (n+3)*a(n) + (-3*n-4)*a(n-1) + (n-4)*a(n-3)
        + 2*(2*n-7)*a(n-4) + (n-6)*a(n-5) + (-n+7)*a(n-6) = 0
   Initial values:
        a(0)=0, a(1)=0, a(2)=0, a(3)=1, a(4)=2, a(5)=5
   The recurrence yields integer values; division is exact.
*/

#include <stdlib.h>

/* Note: seq_t and idx_t are defined by the build system.  seq_t must be
   a type supporting the usual arithmetic operators and should be trivially
   copyable (built‑in or a big‑integer type that follows the trivial
   constructor/destructor convention). */

seq_t a(idx_t n)
{
    /* Static cache for memoisation */
    static seq_t *cache         = NULL;
    static idx_t cache_len      = 0;     /* number of stored elements */

    /* If needed, enlarge the cache to accommodate up to index n */
    if (n + 1 > cache_len) {
        idx_t new_len = n + 1;
        seq_t *new_cache = (seq_t *)realloc(cache, new_len * sizeof(seq_t));
        if (!new_cache) {
            /* Allocation failure – abort (should not happen in tests) */
            exit(1);
        }
        /* Initialise new entries to zero */
        for (idx_t i = cache_len; i < new_len; ++i)
            new_cache[i] = (seq_t)0;
        cache = new_cache;

        /* Set the known initial values if they are within the new size */
        if (new_len > 0)  cache[0] = (seq_t)0;      /* a(0) */
        if (new_len > 1)  cache[1] = (seq_t)0;      /* a(1) */
        if (new_len > 2)  cache[2] = (seq_t)0;      /* a(2) */
        if (new_len > 3)  cache[3] = (seq_t)1;      /* a(3) */
        if (new_len > 4)  cache[4] = (seq_t)2;      /* a(4) */
        if (new_len > 5)  cache[5] = (seq_t)5;      /* a(5) */

        /* Compute missing terms using the recurrence */
        idx_t start = cache_len > 6 ? cache_len : 6;
        for (idx_t i = start; i < new_len; ++i) {
            seq_t term = (seq_t)(3 * i + 4) * cache[i - 1];
            term -= (seq_t)(i - 4) * cache[i - 3];
            term -= (seq_t)(2 * (2 * i - 7)) * cache[i - 4];
            term -= (seq_t)(i - 6) * cache[i - 5];
            if (i >= 7)
                term += (seq_t)(i - 7) * cache[i - 6];
            cache[i] = term / (seq_t)(i + 3);
        }

        cache_len = new_len;
    }

    return cache[n];
}
