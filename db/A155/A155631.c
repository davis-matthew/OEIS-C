/*
 * A155631  a(n) = 10^n-4^n+1^n.
 * https://oeis.org/A155631
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 2, u16 n <= 4, u32 n <= 9, u64 n <= 18, u128 n <= 18,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*
 * Sequence A155631: a(n) = 10^n - 4^n + 1^n
 * Recurrence: a(n) = 14*a(n-1) - 40*a(n-2) + 27, with a(0)=1, a(1)=7.
 */
#include <stdlib.h>

seq_t a(idx_t n)
{
    /* static cache for memoisation */
    static seq_t *cache = NULL;
    static idx_t cap = 0;
    static idx_t size = 0;

    /* helper to grow cache */
    if (cap == 0) {
        cap = (n < 16) ? 16 : n * 2;
        cache = (seq_t *)malloc(cap * sizeof(seq_t));
        if (!cache) { abort(); }
        /* base cases */
        cache[0] = 1;
        cache[1] = 7;
        size = 2;
    } else if (n >= cap) {
        idx_t new_cap = (n < cap * 2) ? cap * 2 : n * 2;
        seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
        if (!tmp) { abort(); }
        cache = tmp;
        cap = new_cap;
    }

    /* compute missing values up to n */
    for (; size <= n; ++size) {
        /* a(i) = 14*a(i-1) - 40*a(i-2) + 27 */
        cache[size] = cache[size - 1] * 14
                    - cache[size - 2] * 40
                      + 27;
    }

    return cache[n];
}
