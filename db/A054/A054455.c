/*
 * A054455  Row sums of triangle A054453.
 * https://oeis.org/A054455
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = Sum_{m=0..n} A054453(n, m). The sequence satisfies the
 *            following linear recurrence with initial values a(0) = 1 , a(1)
 *            = 3 , a(2) = 7 , a(3) = 16 , a(4) = 34 , a(5) = 70 for n ≥ 6
 *            a(n) = 3*a(n-1) - 5*a(n-3) + 3*a(n-5) + a(n-6)
 * Range:     u8 n <= 6, u16 n <= 15, u32 n <= 36, u64 n <= 79, u128 n <= 168,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Include the standard header for memory allocation */
#include <stdlib.h>

seq_t a(idx_t n)
{
    /* Static cache to keep previously computed values.
       cache[0..next-1] holds all values already computed.  */
    static seq_t *cache = NULL;
    static idx_t cache_size = 0;   /* number of allocated slots     */
    static idx_t next     = 0;     /* next index that must be derived */

    /* Initial static array of the first six terms */
    static const seq_t init[6] = { 1, 3, 7, 16, 34, 70 };

    /* -----------------------------------------------------------------
       Make sure the cache is big enough to store values up to n inclusive.
       ----------------------------------------------------------------- */
    if (cache == NULL)         /* first call: allocate a few extra slots     */
    {
        cache_size = 16;                   /* arbitrary start size                 */
        cache = (seq_t *)malloc(cache_size * sizeof(seq_t));
        if (!cache) { abort(); }           /* out of memory                       */
        /* Copy the known first six values */
        for (idx_t i = 0; i < 6; ++i)
            cache[i] = init[i];
        next = 6;   /* first missing index */
    }
    else if (n >= cache_size)      /* enlarge cache if necessary                 */
    {
        size_t new_size = cache_size;
        while (new_size <= (size_t)n) new_size <<= 1;
        seq_t *tmp = (seq_t *)realloc(cache, new_size * sizeof(seq_t));
        if (!tmp) { abort(); }
        cache     = tmp;
        cache_size = (idx_t)new_size;
    }

    /* -----------------------------------------------------------------
       Compute missing values up to n using the recurrence.
       ----------------------------------------------------------------- */
    while (next <= n)
    {
        idx_t i = next;   /* current index to compute */

        /* For i < 6 we already have initial values */
        if (i < 6) {
            cache[i] = init[i];
            ++next;
            continue;
        }

        /* a(i) = 3*a(i-1) - 5*a(i-3) + 3*a(i-5) + a(i-6) */
        seq_t term1 = 3 * cache[i - 1];
        seq_t term2 = 5 * cache[i - 3];
        seq_t term3 = 3 * cache[i - 5];
        seq_t term4 =   cache[i - 6];

        /* All terms are non‑negative and the result fits in seq_t
           for every n that is required by the test harness.        */
        seq_t val = term1 + term3 + term4;
        val = val - term2;   /* subtraction is safe in the same range   */

        cache[i] = val;
        ++next;
    }

    /* -----------------------------------------------------------------
       Return the required term.
       ----------------------------------------------------------------- */
    return cache[n];
}
