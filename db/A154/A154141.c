/*
 * A154141  Indices k such that 8 plus the k-th triangular number is a perfect
 *          square.
 * https://oeis.org/A154141
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 5, u16 n <= 12, u32 n <= 24, u64 n <= 30, u128 n <= 30,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Recurrence: a(1)=1, a(2)=7, a(3)=16, a(4)=46,
   a(n)=6*a(n-2)-a(n-4)+2   for n>4   (OEIS A154141) */
#include <stdlib.h>
#include <stddef.h>

seq_t a(idx_t n)
{
    static seq_t *cache = NULL;   /* 1‑based array, cache[0] unused */
    static idx_t cached_len = 0;  /* highest index computed so far */

    if (n == 0)
        return 0;  /* undefined, but keep safe */

    if (n > cached_len) {
        /* Allocate (n+1) elements to hold indices 0..n */
        seq_t *tmp = (seq_t *)realloc(cache, (n + 1) * sizeof(seq_t));
        if (!tmp)  /* allocation failed */
            return 0;
        cache = tmp;

        /* Compute new terms up to n.  cache_len < n */
        for (idx_t i = cached_len + 1; i <= n; ++i) {
            switch (i) {
                case 1:
                    cache[i] = 1;
                    break;
                case 2:
                    cache[i] = 7;
                    break;
                case 3:
                    cache[i] = 16;
                    break;
                case 4:
                    cache[i] = 46;
                    break;
                default:
                    /* recurrence: a(i)=6*a(i-2)-a(i-4)+2 */
                    cache[i] = 6 * cache[i - 2] - cache[i - 4] + 2;
                    break;
            }
        }
        cached_len = n;
    }

    return cache[n];
}
