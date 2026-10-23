/*
 * A043826  Numbers k such that number of runs in the base 4 representation of
 *          k is congruent to 1 mod 3.
 * https://oeis.org/A043826
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   numbers k such that the number of runs in the base‑4
 *            representation of k is congruent to 1 modulo 3
 * Range:     u8 n <= 52, u16 n <= 52, u32 n <= 52, u64 n <= 52, u128 n <= 52,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n)
{
    /* Static cache of computed values. */
    static seq_t *cache = NULL;
    static size_t cache_len = 0;   /* number of elements stored */
    static size_t cache_cap = 0;   /* allocated capacity */
    static idx_t next_k = 1;       /* next value of k to test */

    /* Ensure the cache contains at least n elements. */
    while (cache_len < n) {
        /* Test the next integer k = next_k. */
        idx_t k = next_k++;
        idx_t m = k;
        uint8_t prev = 4; /* sentinel value outside {0,1,2,3} */
        int runs = 0;

        /* Count runs by processing base‑4 digits from least
           significant to most significant (reversal preserves
           run count).  Using bit shift instead of division
           for speed. */
        while (m) {
            uint8_t d = m & 3;      /* m % 4 */
            if (d != prev) {
                runs++;
                prev = d;
            }
            m >>= 2;                /* m /= 4 */
        }

        /* Store k if runs ≡ 1 (mod 3). */
        if (runs % 3 == 1) {
            if (cache_len == cache_cap) {
                size_t new_cap = (cache_cap == 0) ? 64 : cache_cap * 2;
                seq_t *tmp = (seq_t*)realloc(cache, new_cap * sizeof(seq_t));
                if (!tmp) abort();         /* allocation failure */
                cache = tmp;
                cache_cap = new_cap;
            }
            cache[cache_len++] = (seq_t)k;
        }
    }

    return cache[n - 1];
}
