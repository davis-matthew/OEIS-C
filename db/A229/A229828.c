/*
 * A229828  a(n) = 7*n! - 1.
 * https://oeis.org/A229828
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 7*n! - 1
 * Range:     u8 n <= 4, u16 n <= 7, u32 n <= 12, u64 n <= 20, u128 n <= 33,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computes a(n) = 7 * n! - 1 for all n >= 0.
   The function uses a static cache to store factorials up to the
   largest n requested so far, ensuring that repeated calls are
   answered in O(1) after the first O(n) build.  The cache grows
   as needed using realloc.  All arithmetic is performed using
   the seq_t type, which may be an unsigned integer type of any
   width or an arbitrary‑precision integer class.  This implementation
   is valid both as C11 and as C++17.  */
#include <stdlib.h>

seq_t a(idx_t n)
{
    /* Static cache of factorials: fact_cache[i] = i!   */
    static seq_t *fact_cache = NULL;
    static size_t fact_size = 0;        /* number of cached factorials   */

    /* Ensure cache is at least large enough for n.  */
    if ((size_t)n >= fact_size)
    {
        size_t old_size = fact_size;
        size_t new_size = (size_t)n + 1;

        /* Allocate or grow the cache.  */
        seq_t *new_cache = (seq_t *)realloc(fact_cache, new_size * sizeof(seq_t));
        if (!new_cache)
        {
            /* In a full implementation a fatal error would be handled here.
               For the purposes of this function, we return 0.  */
            return (seq_t)0;
        }
        fact_cache = new_cache;

        /* Initialize factorials that are not yet computed.  */
        if (old_size == 0)
        {
            fact_cache[0] = (seq_t)1;    /* 0! = 1 */
            old_size = 1;
        }

        for (size_t i = old_size; i <= n; ++i)
        {
            fact_cache[i] = fact_cache[i - 1] * (seq_t)i;
        }
        fact_size = new_size;
    }

    /* Compute 7 * n! - 1.  */
    seq_t result = fact_cache[n] * (seq_t)7 - (seq_t)1;
    return result;
}
