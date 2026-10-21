/*
 * A079921  Solution to the Dancing School Problem with n girls and n+2 boys:
 *          f(n,2).
 * https://oeis.org/A079921
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = a(n-1) + a(n-2) + n + 1, a(1) = 3, a(2) = 7. The function
 *            uses a simple memoised recurrence. It works for any n >= 1
 * Range:     u8 n <= 8, u16 n <= 19, u32 n <= 42, u64 n <= 88, u128 n <= 181,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* the build supplies the integer types seq_t and idx_t */
#include <stdlib.h>   /* for malloc/realloc */

/* static cache held between calls.  0‑th entry is unused. */
static seq_t *cache = NULL;
static idx_t  cache_size = 0;

/* Return the nth term of A079921.  */
seq_t a(idx_t n)
{
    /* The sequence is undefined for n = 0; return 0 for safety. */
    if (n == 0)
        return (seq_t)0;

    /* Initialise cache for the first two terms if not already done. */
    if (!cache)
    {
        /* Allocate two plus one for index 0 (unused). */
        cache_size = 2;
        cache = (seq_t *)malloc((cache_size + 2) * sizeof(seq_t));
        cache[0] = (seq_t)0;          /* unused */
        cache[1] = (seq_t)3;          /* a(1) */
        cache[2] = (seq_t)7;          /* a(2) */
    }

    /* Return cached value if already computed. */
    if (n <= cache_size)
        return cache[n];

    /* Grow the cache to accommodate up to index n. */
    idx_t new_size = n;
    cache = (seq_t *)realloc(cache, (new_size + 2) * sizeof(seq_t));

    /* Compute missing terms using the recurrence. */
    for (idx_t i = cache_size + 1; i <= new_size; ++i)
    {
        cache[i] = cache[i - 1] + cache[i - 2] + (seq_t)i + (seq_t)1;
    }

    cache_size = new_size;
    return cache[n];
}
