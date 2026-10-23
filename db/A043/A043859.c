/*
 * A043859  Numbers k such that number of runs in the base 4 representation of
 *          k is congruent to 1 mod 9.
 * https://oeis.org/A043859
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   numbers k such that the number of runs in the base‑4
 *            representation * of k is congruent to 1 modulo 9
 * Range:     u8 n <= 12, u16 n <= 24, u32 n <= 37, u64 n <= 37, u128 n <= 37,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n)
{
    static seq_t *cache = NULL;
    static size_t cache_size = 0;
    static size_t cache_cap = 0;
    static unsigned long long next_k = 1;   /* next integer to test */

    if (n == 0)      /* offset starts at 1; behaviour for 0 is undefined */
        return 0;

    /* Grow the cache until we have at least n terms */
    if (cache_size < n) {
        while (cache_size < n) {
            unsigned long long v = next_k;
            int runs = 0;
            int prev = -1;
            unsigned long long tmp = v;
            /* Count runs in base 4 */
            while (tmp > 0) {
                int d = tmp % 4;
                if (d != prev) {
                    runs++;
                    prev = d;
                }
                tmp /= 4;
            }
            if (runs % 9 == 1) {
                if (cache_size >= cache_cap) {
                    cache_cap = cache_cap ? cache_cap * 2 : 16;
                    seq_t *tmp_alloc = (seq_t *)realloc(cache, cache_cap * sizeof(seq_t));
                    if (!tmp_alloc)   /* allocation failed – return 0 as a fallback */
                        return 0;
                    cache = tmp_alloc;
                }
                cache[cache_size++] = (seq_t)v;
            }
            next_k++;
        }
    }
    return cache[n - 1];
}
