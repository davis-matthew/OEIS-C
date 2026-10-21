/*
 * A235926  Number of (n+1) X (1+1) 0..3 arrays with the minimum plus the
 *          maximum unequal to the lower median plus the upper median in every
 *          2 X 2 subblock.
 * https://oeis.org/A235926
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(1) = 168, a(2) = 1788, a(3) = 18992, a(4) = 201976, a(5) =
 *            2146656, a(6) = 22822704 For n >= 7: a(n) = 82*a(n-2) +
 *            320*a(n-3) + 144*a(n-4) - 448*a(n-5) + 64*a(n-6)
 * Range:     u8 n <= 1, u16 n <= 3, u32 n <= 8, u64 n <= 17, u128 n <= 36,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* ---
   The sequence is linear recurrences of order 6 with the
   coefficients given above.  We memoise all values up to the
   maximum index received, so the function answers any future
   request in O(1) time.
   ---
*/

#include <stdlib.h>
#include <stdint.h>

static seq_t *cache = NULL;      /* 1‑based sequence storage               */
static size_t cache_size = 0;    /* current capacity of cache array         */
static idx_t  cache_max  = 0;    /* largest n that has been computed        */

/* Allocate or grow the cache to hold at least 'needed' entries
   (index 0 is unused).  Uses C realloc; guarantees that the array
   can hold 0...needed-1 positions. */
static void ensure_cache_capacity(size_t needed)
{
    if (needed > cache_size) {
        size_t new_cap = needed * 2;
        seq_t *new_ptr = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
        if (!new_ptr) abort();              /* out of memory */
        cache = new_ptr;
        cache_size = new_cap;
    }
}

/* Initialise the cache with the first six terms of the sequence. */
static void initialise_cache(void)
{
    ensure_cache_capacity(7);                 /* need indices 0..6 inclusive */

    cache[0] = 0;                            /* unused */
    cache[1] = 168;
    cache[2] = 1788;
    cache[3] = 18992;
    cache[4] = 201976;
    cache[5] = 2146656;
    cache[6] = 22822704;
    cache_max = 6;
}

seq_t a(idx_t n)
{
    if (n == 0) return 0;                    /* out of range, default 0 */

    /* Allocate/copy sequence data on first call */
    if (cache == NULL) {
        initialise_cache();
    }

    /* If the requested term is already computed, return it. */
    if (n <= cache_max) {
        return cache[n];
    }

    /* Extend the cache up to 'n' (inclusive). */
    ensure_cache_capacity((size_t)n + 1);

    /* Compute missing terms one by one, using the recurrence. */
    for (idx_t i = cache_max + 1; i <= n; ++i) {
        if (i <= 6) {                       /* safety; should not happen    */
            /* initialize the first six terms if they were not set
               (in case of weird initial state). */
            static const seq_t init[7] = {
                0ULL,         168ULL, 1788ULL,
                18992ULL, 201976ULL, 2146656ULL,
                22822704ULL
            };
            cache[i] = init[i];
        } else {
            seq_t val = 82 * cache[i-2];
            val += 320 * cache[i-3];
            val += 144 * cache[i-4];
            val += 64  * cache[i-6];
            val -= 448 * cache[i-5];          /* guaranteed safe, result > 0 */
            cache[i] = val;
        }
    }

    cache_max = n;
    return cache[n];
}
