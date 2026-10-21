/*
 * A221276  Number of 2 X n arrays of occupancy after each element moves to
 *          some horizontal, diagonal or antidiagonal neighbor, with every
 *          occupancy equal to zero or two.
 * https://oeis.org/A221276
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 4*a(n-1) - 12*a(n-3) + 9*a(n-4) for n >= 5, with a(1)=0,
 *            a(2)=4, a(3)=4, a(4)=25
 * Range:     u8 n <= 6, u16 n <= 11, u32 n <= 21, u64 n <= 41, u128 n <= 82,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdlib.h>

#ifdef __cplusplus
/* Helper: allocate an array of seq_t of given capacity (0..cap) */
static seq_t *alloc_array(size_t cap)
{
    return new seq_t[cap + 1];
}
/* Helper: free the array allocated by alloc_array */
static void free_array(seq_t *ptr)
{
    delete [] ptr;
}
#else
static seq_t *alloc_array(size_t cap)
{
    return (seq_t *)malloc((cap + 1) * sizeof(seq_t));
}
static void free_array(seq_t *ptr)
{
    free(ptr);
}
#endif

seq_t a(idx_t n)
{
    if (n < 1)   /* offset is 1; behavior for n<1 is irrelevant */
        return (seq_t)0;

    /* Static cache: 1‑based indices. */
    static seq_t *cache = NULL;
    static size_t cache_capacity = 0;
    static size_t cache_size = 0;   /* highest index computed */

    /* If we already have the value, just return it. */
    if (n <= cache_size)
        return cache[(size_t)n];

    /* Ensure cache can hold all indices up to n. */
    for (size_t i = cache_size + 1; i <= (size_t)n; ++i)
    {
        /* Grow the array if necessary. */
        if (i > cache_capacity)
        {
            size_t new_cap = cache_capacity ? cache_capacity * 2 : 4;
            while (new_cap < i) new_cap <<= 1;

            seq_t *new_cache = alloc_array(new_cap);

            if (cache != NULL)
            {
                /* Copy existing values. */
                for (size_t k = 1; k <= cache_size; ++k)
                    new_cache[k] = cache[k];
                free_array(cache);
            }
            cache = new_cache;
            cache_capacity = new_cap;
        }

        /* Compute a(i).  Base values for i <= 4 are hard‑coded. */
        seq_t val;
        if (i == 1)        val = (seq_t)0;
        else if (i == 2)   val = (seq_t)4;
        else if (i == 3)   val = (seq_t)4;
        else if (i == 4)   val = (seq_t)25;
        else
        {
            /* Recurrence: a(i) = 4*a(i-1) - 12*a(i-3) + 9*a(i-4) */
            seq_t term1 = (seq_t)4 * cache[i - 1];
            seq_t term2 = (seq_t)9 * cache[i - 4];
            seq_t term3 = (seq_t)12 * cache[i - 3];
            val = term1 + term2 - term3;
        }

        cache[i] = val;
        cache_size = i;
    }

    return cache[(size_t)n];
}
