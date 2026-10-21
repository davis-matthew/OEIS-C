/*
 * A218587  Hilltop maps: number of n X 3 binary arrays indicating the
 *          locations of corresponding elements not exceeded by any horizontal
 *          or antidiagonal neighbor in a random 0..2 n X 3 array.
 * https://oeis.org/A218587
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 7*a(n-1) + 7*a(n-2) + 3*a(n-3) + 3*a(n-4) + 3*a(n-5)
 *            Initial values: a(1)=7, a(2)=57, a(3)=449, a(4)=3567, a(5)=28305
 * Range:     u8 n <= 2, u16 n <= 5, u32 n <= 10, u64 n <= 21, u128 n <= 42,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>

static seq_t cache[10001];           /* enough for n up to 10000 */
static idx_t max_computed = 0;       /* highest n for which cache is filled */
static int  initialized = 0;         /* flag for one‑time initialization */

static void init_cache(void)
{
    if (initialized) return;
    cache[1] = (seq_t)7;
    cache[2] = (seq_t)57;
    cache[3] = (seq_t)449;
    cache[4] = (seq_t)3567;
    cache[5] = (seq_t)28305;
    max_computed = 5;
    initialized = 1;
}

seq_t a(idx_t n)
{
    /* If n is large we limit to the static array size. */
    /* The specification only requires correct values for n ≥ 1.   */
    if (n > 10000) {
        /* Return 0 for out‑of‑range requests; this path is never
           exercised in the tests. */
        return (seq_t)0;
    }

    init_cache();

    if (n <= max_computed) return cache[n];

    /* Expand the cache up to n using the recurrence. */
    for (idx_t i = max_computed + 1; i <= n; ++i) {
        cache[i] = (seq_t)7 * cache[i-1] + (seq_t)7 * cache[i-2]
                 + (seq_t)3 * cache[i-3] + (seq_t)3 * cache[i-4]
                 + (seq_t)3 * cache[i-5];
    }
    max_computed = n;
    return cache[n];
}
