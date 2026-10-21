/*
 * A099676  Partial sums of repdigits of A002283.
 * https://oeis.org/A099676
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  base, nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 2, u16 n <= 4, u32 n <= 9, u64 n <= 19, u128 n <= 38,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence: A099676 – Partial sums of repdigits of A002283
 *
 * The terms satisfy the recurrence
 *   a(1) = 9
 *   a(2) = 108
 *   a(3) = 1107
 *   a(n) = 12*a(n-1) - 21*a(n-2) + 10*a(n-3)   (n >= 4).
 *
 * This is equivalent to the closed‑form expression
 *   a(n) = (10/9)*(10^n - 1) - n.
 */

#include <stdlib.h>

seq_t a(idx_t n)
{
    /* Cache computed values to allow arbitrary call order. */
    static seq_t *cache = NULL;
    static idx_t cache_len = 0;

    if (n == 0)
        return (seq_t)0;        /* undefined for offset 1, but safe */

    /* Ensure the cache can hold up to a(n). */
    if (cache_len < n) {
        size_t new_len = (size_t)n;
        cache = (seq_t *)realloc(cache, new_len * sizeof(seq_t));
        if (cache == NULL)
            abort();           /* allocation failure */

        /* Fill in new terms. */
        for (idx_t i = cache_len + 1; i <= n; ++i) {
            seq_t val;
            if (i == 1)
                val = (seq_t)9;
            else if (i == 2)
                val = (seq_t)108;
            else if (i == 3)
                val = (seq_t)1107;
            else
                val = (seq_t)12 * cache[i - 2]      /* a(i-1)  */
                    - (seq_t)21 * cache[i - 3]     /* a(i-2)  */
                    + (seq_t)10 * cache[i - 4];    /* a(i-3)  */
            cache[i - 1] = val;
        }
        cache_len = n;
    }

    return cache[n - 1];
}
