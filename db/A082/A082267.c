/*
 * A082267  Number of palindromes that use nonzero digits and have a digit sum
 *          of n.
 * https://oeis.org/A082267
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  base, nonn
 * Formula:   a(1) = 1, a(2) = 2 For n > 2: a(n) = 2*a(n‑2) if n < 10 or 12 ≤
 *            n ≤ 19 a(n) = 2*a(n‑2) – 1 if n = 10 or 11 or 20 a(n) = 2*a(n‑2)
 *            – a(n‑20) if n ≥ 21
 * Range:     u8 n <= 17, u16 n <= 33, u32 n <= 65, u64 n <= 129,
 *            u128 n <= 257, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n)
{
    if (n == 0)
        return 0;            /* undefined for n = 0, never requested */

    /* Large static cache – indices 1..max_n are filled progressively. */
    static seq_t *cache = NULL;
    static idx_t  cache_size = 0;          /* current highest computed index */

    if (n > cache_size) {
        idx_t newmax = n;
        seq_t *tmp = (seq_t *)realloc(cache, (newmax + 1) * sizeof(seq_t));
        if (!tmp)         /* allocation failure (unlikely for given limits) */
            return 0;
        cache = tmp;

        for (idx_t i = cache_size + 1; i <= newmax; ++i) {
            if (i == 1)
                cache[1] = 1;
            else if (i == 2)
                cache[2] = 2;
            else if (i == 10 || i == 11 || i == 20)
                cache[i] = 2 * cache[i - 2] - 1;
            else if (i >= 21)
                cache[i] = 2 * cache[i - 2] - cache[i - 20];
            else        /* 3 ≤ i ≤ 9 or 12 ≤ i ≤ 19 */
                cache[i] = 2 * cache[i - 2];
        }
        cache_size = newmax;
    }

    return cache[n];
}
