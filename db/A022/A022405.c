/*
 * A022405  a(n) = a(n-1)*a(n-2) - a(n-3), with a(1) = 0, a(2) = 1, a(3) = 2.
 * https://oeis.org/A022405
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  easy, nonn
 * Formula:   not stated
 * Range:     u8 n <= 8, u16 n <= 10, u32 n <= 11, u64 n <= 13, u128 n <= 14,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Recurrence:  a(1)=0, a(2)=1, a(3)=2,
   a(n)=a(n-1)*a(n-2)-a(n-3)  for n ≥ 4. */
#include <stdlib.h>

seq_t a(idx_t n)
{
    /* static cache for memoised values */
    static seq_t *cache   = (seq_t*)0;
    static size_t capacity   = 0;      /* allocated slots (index 0 unused) */
    static idx_t  max_computed = 0;    /* highest index for which cache[ ] is valid */

    if (n < 1)
        return (seq_t)0;   /* undefined for n<1, return 0 for safety */

    /* enlarge cache if necessary */
    if ((size_t)n + 1 > capacity)
    {
        size_t new_cap = capacity ? capacity : 4;
        while (new_cap <= (size_t)n)
            new_cap <<= 1;

        seq_t *tmp = (seq_t*)realloc(cache, new_cap * sizeof(seq_t));
        if (!tmp)
            abort();   /* out of memory; terminate the program */
        cache = tmp;
        capacity = new_cap;
    }

    /* compute all missing values up to n */
    for (idx_t i = max_computed + 1; i <= n; ++i)
    {
        if (i == 1)
            cache[i] = (seq_t)0;
        else if (i == 2)
            cache[i] = (seq_t)1;
        else if (i == 3)
            cache[i] = (seq_t)2;
        else
            cache[i] = cache[i-1] * cache[i-2] - cache[i-3];
    }

    if (n > max_computed)
        max_computed = n;

    return cache[n];
}
