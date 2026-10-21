/*
 * A066983  a(n+2) = a(n+1) + a(n) + (-1)^n, with a(1) = a(2) = 1.
 * https://oeis.org/A066983
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = a(n-1) + a(n-2) + (-1)^n (a(1) = a(2) = 1)
 * Range:     u8 n <= 13, u16 n <= 25, u32 n <= 48, u64 n <= 94, u128 n <= 186,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n)
{
    static seq_t *cache = NULL;      /* cache[1..k] holds a(1)..a(k)   */
    static idx_t max_n   = 0;        /* largest n already computed     */

    if (n <= 0) return 0;            /* undefined, but safe to return 0 */

    /* Grow the cache if needed */
    if (n > max_n)
    {
        idx_t new_cap = n + 1;       /* allocate indices 0..n; ignore 0        */
        seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
        if (!tmp) return 0;           /* allocation failed – just abort         */
        cache = tmp;

        /* Initialise the new entries */
        for (idx_t i = max_n + 1; i <= n; ++i)
        {
            if (i == 1 || i == 2)
            {
                cache[i] = (seq_t)1;
            }
            else
            {
                seq_t sum = cache[i-1] + cache[i-2];
                if ((i & 1) == 0)      /* i even  → (+1) */
                    sum += (seq_t)1;
                else                   /* i odd   → (-1) */
                    sum -= (seq_t)1;
                cache[i] = sum;
            }
        }
        max_n = n;
    }
    return cache[n];
}
