/*
 * A053142  a(n) = A053141(n)/2.
 * https://oeis.org/A053142
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(0)=0, a(1)=1, a(n) = 6*a(n-1) - a(n-2) + 1 (for n ≥ 2)
 * Range:     u8 n <= 4, u16 n <= 7, u32 n <= 13, u64 n <= 26, u128 n <= 51,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* The sequence is computed iteratively with a static cache.
   A fixed-size array is sufficient for all n ≤ 10000. */

static seq_t cache[10001] = {0};   /* static storage, zero-initialized   */
static idx_t cache_size = 0;      /* number of elements that have been
                                      computed and stored in cache      */

seq_t a(idx_t n)
{
    /* Initialise the first two terms once. */
    if (cache_size == 0)
    {
        cache[0] = (seq_t)0;
        cache[1] = (seq_t)1;
        cache_size = 2;
    }

    /* Extend the cache up to n using the recurrence. */
    while (cache_size <= n)
    {
        /* cache_size is the first index that is not yet computed. */
        idx_t i = cache_size;             /* i ≥ 2 */
        cache[i] = (seq_t)6 * cache[i - 1] - cache[i - 2] + (seq_t)1;
        ++cache_size;
    }

    return cache[n];
}
