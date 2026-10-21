/*
 * A231003  Number of years after which it is not possible to have a date
 *          falling on the same day of the week, in the Julian calendar.
 * https://oeis.org/A231003
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 73, u16 n <= 73, u32 n <= 73, u64 n <= 73, u128 n <= 73,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* OEIS A231003:
 *  Recurrence: a(n) = a(n-1) + a(n-21) - a(n-22)   for n > 22
 *  initial values: a(1)=1, a(2)=2, a(3)=3, a(4)=4, a(5)=7, a(6)=8, a(7)=9,
 *  a(8)=10, a(9)=12, a(10)=13, a(11)=14, a(12)=15, a(13)=16, a(14)=18,
 *  a(15)=19, a(16)=20, a(17)=21, a(18)=24, a(19)=25, a(20)=26, a(21)=27,
 *  a(22)=29
 *  The sequence is periodic with period 28, but we compute it by the recurrence
 *  using a memoising cache that works for any access order.
 */

#include <stdlib.h>
#include <stdbool.h>

/* initial 22 terms (index 0 unused) */
static const seq_t init_vals[22] = {
    1, 2, 3, 4, 7, 8, 9, 10, 12, 13,
    14, 15, 16, 18, 19, 20, 21, 24, 25, 26,
    27, 29
};

/* cache[0] is unused, cache[1..cache_max] contain known values */
static seq_t *cache      = NULL;
static idx_t   cache_cap = 0;   /* size of allocated array */
static idx_t   cache_max = 0;   /* maximum index currently defined */

/* enlarge cache to hold indices up to 'required' */
static bool grow_cache(idx_t required)
{
    if (required < cache_cap) return true;           /* already enough */

    idx_t new_cap = required + 1;                    /* include index 0 */
    if (new_cap < 64) new_cap = 64;                  /* minimum size */

    seq_t *new_cache = (seq_t *)malloc((size_t)new_cap * sizeof(seq_t));
    if (!new_cache) return false;                    /* allocation failed */

    /* copy existing contents (works for trivial and non‑trivial types) */
    for (idx_t i = 0; i < cache_cap; ++i)
        new_cache[i] = cache[i];

    free(cache);
    cache      = new_cache;
    cache_cap  = new_cap;
    return true;
}

/* ensure that all values up to n are computed */
static bool compute_to(idx_t n)
{
    if (!grow_cache(n))
        return false;                               /* memory failure */

    /* initialise first 22 terms once */
    if (cache_max < 22) {
        for (idx_t i = 1; i <= 22; ++i)
            cache[i] = init_vals[i-1];
        cache_max = 22;
    }

    /* compute missing values */
    for (idx_t i = cache_max + 1; i <= n; ++i)
        cache[i] = cache[i-1] + cache[i-21] - cache[i-22];

    cache_max = n;
    return true;
}

/* public function required by the test harness */
seq_t a(idx_t n)
{
    if (n == 0) return 0;                           /* undefined, keep safe */

    if (n <= 22) return init_vals[n-1];

    if (!compute_to(n))
        return 0;                                   /* out of memory */

    return cache[n];
}
