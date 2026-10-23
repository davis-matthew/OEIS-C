/*
 * A037671  Base 8 digits are, in order, the first n terms of the periodic
 *          sequence with initial period 3,2,0.
 * https://oeis.org/A037671
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base, easy
 * Formula:   a(n) = 8*a(n-1)+a(n-3)-8*a(n-4), a(1)=3, a(2)=26, a(3)=208,
 *            a(4)=1667
 * Range:     u8 n <= 3, u16 n <= 5, u32 n <= 11, u64 n <= 21, u128 n <= 43,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdlib.h>   /* realloc, abort */

seq_t a(idx_t n)
{
    if (n == 0)
        return 0;   /* undefined for n=0, but return 0 safely */

    static seq_t *cache = NULL;  /* cache[1 .. cached_n] holds the terms */
    static idx_t cached_n = 0;   /* maximum n already computed */

    if (n > cached_n) {
        /* grow the cache to hold indices 0 .. n (index 0 unused) */
        size_t new_size = (size_t)(n + 1);   /* size_t is guaranteed at least as large as idx_t */
        seq_t *tmp = (seq_t*)realloc(cache, new_size * sizeof(seq_t));
        if (!tmp)
            abort();  /* allocation failed */
        cache = tmp;

        /* compute new terms using the recurrence */
        for (idx_t i = cached_n + 1; i <= n; ++i) {
            if (i == 1)
                cache[i] = 3;
            else if (i == 2)
                cache[i] = 26;
            else if (i == 3)
                cache[i] = 208;
            else if (i == 4)
                cache[i] = 1667;
            else
                cache[i] = 8 * cache[i - 1] + cache[i - 3] - 8 * cache[i - 4];
        }
        cached_n = n;
    }
    return cache[n];
}
