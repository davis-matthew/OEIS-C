/*
 * A185106  Column 4 of A181783.
 * https://oeis.org/A185106
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 6*n*a(n-1) - 11*n*(n-1)*a(n-2) + 6*n*(n-1)*(n-2)*a(n-3) +
 *            1 with initial values a(0)=1, a(1)=7, a(2)=63
 * Range:     u8 n <= 2, u16 n <= 4, u32 n <= 8, u64 n <= 14, u128 n <= 25,
 *            big any n
 * Helpers:   ensure_capacity
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdint.h>
#include "../utils/ensure_capacity.h"

seq_t a(idx_t n)
{
    /* static cache */
    static seq_t *cache = NULL;
    static idx_t  computed = 0;   /* number of terms already stored (0 .. computed-1) */
    static size_t cap = 0;        /* current capacity of the cache array */

    /* enlarge capacity if needed */
    if ((size_t)n >= cap) {
        size_t need = (size_t)n + 1;
        ensure_capacity(need, &cap, (void **)&cache, sizeof(seq_t));
    }

    /* compute missing terms */
    for (idx_t i = computed; i <= n; ++i) {
        if (i == 0) {
            cache[0] = (seq_t)1;
        } else if (i == 1) {
            cache[1] = (seq_t)7;
        } else if (i == 2) {
            cache[2] = (seq_t)63;
        } else {
            seq_t prod1 = (seq_t)6 * (seq_t)i * cache[i-1];
            seq_t prod2 = (seq_t)11 * (seq_t)i * (seq_t)(i-1) * cache[i-2];
            seq_t prod3 = (seq_t)6 * (seq_t)i * (seq_t)(i-1) * (seq_t)(i-2) * cache[i-3];

            seq_t term = prod1 + prod3 + 1;
            term = term - prod2;
            cache[i] = term;
        }
    }

    computed = n + 1;
    return cache[n];
}
