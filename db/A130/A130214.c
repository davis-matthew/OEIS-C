/*
 * A130214  Order of modular group of degree 5^(n-1)+1.
 * https://oeis.org/A130214
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 5^(n-1) * (5^(2 n - 2) - 1) / 2 Recurrence: a(n) =
 *            130*a(n-1) - 625*a(n-2), a(1)=0, a(2)=60
 * Range:     u8 n <= 2, u16 n <= 3, u32 n <= 5, u64 n <= 10, u128 n <= 15,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n)
{
    static seq_t *cache = NULL;
    static idx_t used = 0;          /* highest index computed in cache */

    /* ensure base cases a(1) and a(2) are available */
    if (n <= 2) {
        if (used < 2) {
            cache = (seq_t *)realloc(cache, sizeof(seq_t) * 3);
            if (!cache) abort();
            cache[0] = 0;
            cache[1] = 0;
            cache[2] = 60;
            used = 2;
        }
        return cache[n];
    }

    /* compute required values up to n if not already cached */
    if (used < n) {
        cache = (seq_t *)realloc(cache, sizeof(seq_t) * (n + 1));
        if (!cache) abort();
        for (idx_t i = used + 1; i <= n; ++i) {
            cache[i] = (seq_t)130 * cache[i - 1] - (seq_t)625 * cache[i - 2];
        }
        used = n;
    }

    return cache[n];
}
