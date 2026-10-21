/*
 * A229514  Number of n X 1 0..2 arrays of the median of the corresponding
 *          element, the element to the east and the element to the south in a
 *          larger (n+1) X 2 0..2 array without adjacent equal elements in the
 *          latter.
 * https://oeis.org/A229514
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 2*a(n-1) + a(n-2) - a(n-3) + a(n-4) (OEIS A229514) * with
 *            initial terms * a(1) = 3, a(2) = 7, a(3) = 17, a(4) = 39 * *
 *            This implementation memoises the values up to n = 10000. It is *
 *            fully generic: the build supplies the types idx_t and seq_t, and
 *            * the function is defined with external l...
 * Range:     u8 n <= 6, u16 n <= 12, u32 n <= 25, u64 n <= 52, u128 n <= 104,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>   /* for size_t */

#define MAX_N 10000

/* Static cache: 1-based indexing, cache_len holds the greatest n
   that has been computed so far.  The array is large enough for the
   maximum n required by the tests.  It is zero-initialised. */
static seq_t cache[MAX_N + 1];
static idx_t cache_len = 0;

/* Initialise the first four terms once.  Subsequent calls will
   preserve the values. */
static void init_base_terms(void)
{
    if (cache_len >= 4)
        return;           /* already initialised */

    cache[1] = (seq_t)3;
    cache[2] = (seq_t)7;
    cache[3] = (seq_t)17;
    cache[4] = (seq_t)39;
    cache_len = 4;
}

/* Public sequence function.  It is safe to call in any order
   and for any n ≥ 1 (values for n > MAX_N are undefined). */
seq_t a(idx_t n)
{
    if (n == 0)
        return (seq_t)0;          /* the sequence is defined only for n ≥ 1 */

    /* Guard against calls beyond the preallocated cache.  Realistic
       tests will never exceed MAX_N. */
    if (n > (idx_t)MAX_N)
        return (seq_t)0;

    init_base_terms();

    /* Return cached value if it already exists. */
    if (n <= cache_len)
        return cache[n];

    /* Compute the missing terms up to n using the empirical recurrence. */
    for (idx_t i = cache_len + 1; i <= n; ++i)
    {
        seq_t val = (seq_t)2 * cache[i - 1];
        val += cache[i - 2];
        val -= cache[i - 3];
        val += cache[i - 4];
        cache[i] = val;
    }
    cache_len = n;
    return cache[n];
}
