/*
 * A188986  Number of n X 3 binary arrays without the pattern 0 0 1
 *          antidiagonally or horizontally.
 * https://oeis.org/A188986
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 6*a(n-1) + a(n-2) – 4*a(n-3) + 2*a(n-4) with initial
 *            values a(1)=7, a(2)=49, a(3)=295, a(4)=1793
 * Range:     u8 n <= 2, u16 n <= 5, u32 n <= 12, u64 n <= 24, u128 n <= 49,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>   /* for malloc, realloc, abort */

static seq_t *cache = NULL;         /* cache[0] unused, indices start at 1 */
static idx_t capacity = 0;          /* allocated capacity (max index)   */
static idx_t computed_len = 0;      /* highest index computed by recurrence */

/* Ensure enough space for indices up to needed-1 and initialise the first
 * four terms if they are missing.
 */
static void ensure_cap(idx_t needed)
{
    if (capacity < needed)
    {
        idx_t new_cap = capacity ? capacity : needed;
        while (new_cap < needed)
            new_cap <<= 1;

#ifdef __cplusplus
        seq_t* tmp = new seq_t[new_cap + 1];
        for (idx_t i = 1; i <= capacity; ++i)
            tmp[i] = cache[i];
        delete [] cache;
        cache = tmp;
        capacity = new_cap;
#else
        seq_t* tmp = (seq_t*)realloc(cache,
                                     (new_cap + 1) * sizeof(seq_t));
        if (!tmp)
            abort();
        cache = tmp;
        capacity = new_cap;
#endif
    }

    if (computed_len < 1 && capacity >= 1)
        cache[1] = (seq_t)7;
    if (computed_len < 2 && capacity >= 2)
        cache[2] = (seq_t)49;
    if (computed_len < 3 && capacity >= 3)
        cache[3] = (seq_t)295;
    if (computed_len < 4 && capacity >= 4)
        cache[4] = (seq_t)1793;

    if (computed_len < 4)
        computed_len = 4;
}

seq_t a(idx_t n)
{
    if (n < 1)
        return (seq_t)0;          /* undefined for n < 1 */

    if (n <= 4)
    {
        ensure_cap(4);
        return cache[n];
    }

    ensure_cap(n + 1);            /* need indices up to n */

    if (computed_len < n)
    {
        for (idx_t i = computed_len + 1; i <= n; ++i)
        {
            cache[i] =
                (seq_t)6 * cache[i - 1]
              + cache[i - 2]
              - (seq_t)4 * cache[i - 3]
              + (seq_t)2 * cache[i - 4];
        }
        computed_len = n;
    }

    return cache[n];
}
