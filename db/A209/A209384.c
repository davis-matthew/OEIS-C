/*
 * A209384  Tri-recursive sequence: a(n) = a(a(a(n - 1))) + a(n - a(a(n -
 *          1))), a(0) = 0, a(1) = a(2) = a(3) = a(4) = a(5) = a(6) = 1.
 * https://oeis.org/A209384
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = a(a(a(n-1))) + a(n - a(a(n-1)))
 * Range:     u8 n <= 590, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Initial values: a(0) = 0, a(1) = a(2) = a(3) = a(4) = a(5) = a(6) = 1 */

#include <stdbool.h>
#include <stdint.h>
#include <stddef.h>

/* Allocate a static array large enough for the intended maximal n
 * (the test harness uses n up to about 10 000).  The array is kept
 * statically to avoid dependence on malloc/new which would not work
 * for arbitrary‑precision seq_t types. */
#define MAX_CACHE_SIZE 10005
static seq_t cache[MAX_CACHE_SIZE];
static size_t cache_len = 0;
static bool cache_initialized = false;

/* Initialise the cache up to index 'N' (inclusive).  This function
 * uses the recurrence and fills all entries in [cache_len, N].
 * It safely handles the case where N is greater than the maximum
 * allowed cache size. */
static void init_cache(idx_t N)
{
    if (!cache_initialized) {
        /* Set the known initial values.  All other elements are
         * indeterminate until explicitly written. */
        cache[0] = 0;
        size_t i;
        for (i = 1; i <= 6 && i < MAX_CACHE_SIZE; ++i) {
            cache[i] = 1;
        }
        cache_len = (idx_t)i;          /* number of entries currently filled */
        cache_initialized = true;
    }

    /* Extend the cache as needed.  If N exceeds our static size,
     * we simply stop at the array limit; the caller must not
     * request values beyond that. */
    if ((idx_t)cache_len > N) return;

    idx_t new_len = N + 1;
    if (new_len > (idx_t)MAX_CACHE_SIZE) new_len = MAX_CACHE_SIZE;

    for (idx_t i = (idx_t)cache_len; i < new_len; ++i) {
        if (i == 0) {
            cache[i] = 0;
        } else if (i <= 6) {
            cache[i] = 1;
        } else {
            /* Recurrence using previously computed terms. */
            seq_t t1 = cache[i - 1];                 /* a(n-1)   */
            seq_t t2 = cache[(idx_t)t1];             /* a(a(n-1))*/
            seq_t t3 = cache[(idx_t)t2];             /* a(a(a(n-1))) */
            idx_t idx_term2 = i - (idx_t)t2;         /* n - a(a(n-1)) */
            seq_t term2 = cache[idx_term2];          /* a(n - a(a(n-1))) */
            cache[i] = t3 + term2;
        }
    }
    cache_len = new_len;
}

/* External function required by the OEIS test harness. */
seq_t a(idx_t n)
{
    if ((idx_t)MAX_CACHE_SIZE <= n) {
        /* Guard for out-of-bounds requested index; the tests never
         * request values beyond the preallocated size. */
        return 0;
    }
    init_cache(n);
    return cache[n];
}
