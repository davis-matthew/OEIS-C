/*
 * A236324  Number of (n+1) X (2+1) 0..1 arrays colored with the sum of the
 *          upper and lower median values of each 2 X 2 subblock.
 * https://oeis.org/A236324
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = a(n-3) + 4*a(n-4) for n > 5; initial values: a(1)=32,
 *            a(2)=44, a(3)=60, a(4)=92, a(5)=156
 * Range:     u8 n <= 6, u16 n <= 19, u32 n <= 45, u64 n <= 97, u128 n <= 200,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n)
{
    static seq_t *cache = NULL;          /* cached sequence, 1‑based */
    static size_t cache_cap = 0;         /* currently allocated capacity */
    static idx_t  last_computed = 0;     /* largest index that has been set */

    if (n == 0) return 0;                /* offset starts at 1 */

    /* Ensure sufficient capacity */
    if ((size_t)n >= cache_cap) {
        size_t new_cap = (size_t)n + 1; /* need space for indices 1..n */
        seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
        if (!tmp) return 0;            /* allocation failure */
        cache = tmp;
        cache_cap = new_cap;
    }

    /* Compute all missing values in order */
    for (idx_t i = last_computed + 1; i <= n; ++i) {
        if (i == 1) cache[1] = (seq_t)32;
        else if (i == 2) cache[2] = (seq_t)44;
        else if (i == 3) cache[3] = (seq_t)60;
        else if (i == 4) cache[4] = (seq_t)92;
        else if (i == 5) cache[5] = (seq_t)156;
        else
            cache[i] = cache[i-3] + (seq_t)4 * cache[i-4];
    }

    last_computed = n;
    return cache[n];
}
