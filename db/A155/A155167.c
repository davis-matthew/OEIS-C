/*
 * A155167  (L)-sieve transform of A004767 = {3,7,11,15,...,4n-1,...}.
 * https://oeis.org/A155167
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = floor((4*a(n-1)+3)/3) ; a(1)=1
 * Range:     u8 n <= 16, u16 n <= 36, u32 n <= 40, u64 n <= 40, u128 n <= 40,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n)
{
    if (n < 1)
        return (seq_t)0;                 /* undefined for n < 1 */

    static seq_t *cache   = NULL;          /* array of a(k) for k=1..cache_size */
    static size_t cache_cap   = 0;         /* allocated capacity */
    static size_t cache_size  = 0;         /* number of elements stored */

    /* Grow cache until it contains at least n elements. */
    if (n > cache_size) {
        /* Ensure enough capacity.  Double until we can hold n elements. */
        size_t need = n;
        if (cache_cap < need) {
            size_t new_cap = (cache_cap == 0) ? 16 : cache_cap;
            while (new_cap < need)
                new_cap <<= 1;                 /* new_cap *= 2 */
            seq_t *tmp = (seq_t *)realloc(cache,
                                          new_cap * sizeof(seq_t));
            if (!tmp && need > 0)               /* allocation failure */
                return (seq_t)0;
            cache = tmp;
            cache_cap = new_cap;
        }
        /* Compute missing terms using the recurrence.  The first term
           (a(1)) is set separately.  All arithmetic uses only
           integer operations with seq_t operands. */
        for (size_t i = cache_size; i < n; ++i) {
            if (i == 0) {                     /* a(1) */
                cache[i] = (seq_t)1;
            } else {
                seq_t prev = cache[i - 1];                /* a(i) since i is 0‑based */
                /* a(i+1) = floor((4*prev+3)/3) =
                   (prev/3)*4 + ((prev%3)*4/3) + 1          */
                seq_t next = (prev / 3) * 4
                           + ((prev % 3) * 4 / 3) + 1;
                cache[i] = next;
            }
        }
        cache_size = n;
    }

    return cache[n - 1];          /* a(n) stored at index n-1 */
}
