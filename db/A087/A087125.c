/*
 * A087125  Indices k of hex numbers H(k) that are also triangular.
 * https://oeis.org/A087125
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(0)=0, a(1)=5, a(n)=10*a(n-1)-a(n-2)+4 (n>1)
 * Range:     u8 n <= 2, u16 n <= 5, u32 n <= 9, u64 n <= 19, u128 n <= 38,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n)
{
    static seq_t *cache    = NULL;
    static idx_t  count    = 0;   /* number of terms currently cached */
    static idx_t  capacity = 0;   /* allocated array size              */

    /* If the requested index is already cached, return it. */
    if (n < count)
        return cache[(size_t)n];

    /* Expand cache if needed. */
    if (capacity < n + 1) {
        idx_t newcap = capacity ? capacity << 1 : 2;
        while (newcap < n + 1)
            newcap <<= 1;
        seq_t *tmp = (seq_t *)realloc(cache, (size_t)newcap * sizeof(seq_t));
        if (!tmp)
            return 0;  /* allocation failure – return 0 as a safe fallback */
        cache    = tmp;
        capacity = newcap;
    }

    /* Compute missing terms up to n using the recurrence. */
    for (idx_t i = count; i <= n; ++i) {
        if (i == 0)
            cache[i] = (seq_t)0;
        else if (i == 1)
            cache[i] = (seq_t)5;
        else
            cache[i] = (seq_t)10 * cache[i - 1] - cache[i - 2] + (seq_t)4;
    }
    count = n + 1;
    return cache[(size_t)n];
}
