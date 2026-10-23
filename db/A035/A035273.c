/*
 * A035273  One quarter of deca-factorial numbers.
 * https://oeis.org/A035273
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  easy, nonn
 * Formula:   4*a(n) = Π_{k=1..n} (10*k – 6)
 * Range:     u8 n <= 2, u16 n <= 4, u32 n <= 7, u64 n <= 12, u128 n <= 20,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n)
{
    /* Cache computed terms to serve out‑of‑order calls. */
    static seq_t *cache = NULL;
    static idx_t  max_computed = 0;   /* highest n whose value is in cache */

    if (n < 1)                 /* offset is 1; values for n < 1 are undefined */
        return 0;

    /* Allocate or grow the cache as needed. */
    if (n > max_computed) {
        /* (1) first allocation or (2) growth of the array.  The array
         * is indexed from 0 to n inclusive; index 0 is unused. */
        seq_t *tmp = (seq_t *)realloc(cache, (n + 1) * sizeof(seq_t));
        if (!tmp) return 0;   /* allocation failure – return 0 as a safe fallback */
        cache = tmp;

        /* (2.1) If this is the very first allocation, initialise index 1. */
        if (max_computed == 0) {
            cache[0] = 0;          /* unused, set to zero for safety */
            cache[1] = 1;          /* a(1) = 1 */
            max_computed = 1;
        }

        /* (2.2) Compute the missing terms up to n. */
        for (idx_t i = max_computed + 1; i <= n; ++i) {
            /* 10*i - 6 fits in idx_t for the problem limits (n ≤ 10 000). */
            seq_t factor = (seq_t)(10 * i - 6);
            cache[i] = cache[i - 1] * factor;
        }
        max_computed = n;
    }

    return cache[n];
}
