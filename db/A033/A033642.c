/*
 * A033642  Base 5 digital convolution sequence.
 * https://oeis.org/A033642
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  base, nonn
 * Formula:   a(0)=1; for n>0, if the base‑5 expansion of n is (... d2 d1 d0)
 *            with d0 the least‑significant digit, then a(n)=∑_{i=0}^{k-1} d_i
 *            * a(n-(i+1))
 * Range:     u8 n <= 8, u16 n <= 13, u32 n <= 23, u64 n <= 29, u128 n <= 29,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>

seq_t a(idx_t n)
{
    static seq_t *cache = NULL;
    static size_t cache_len = 0;   /* number of elements in cache */

    if (cache == NULL) {
        cache = (seq_t *)malloc(sizeof(seq_t));
        cache_len = 0;
    }

    if (n >= (idx_t)cache_len) {
        /* grow cache */
        cache = (seq_t *)realloc(cache, ((size_t)n + 1) * sizeof(seq_t));
        for (size_t i = cache_len; i <= (size_t)n; ++i) {
            if (i == 0) {
                cache[i] = (seq_t)1;
                continue;
            }

            idx_t t = (idx_t)i;
            uint8_t rev[32];   /* enough for base‑5 digits of any 64‑bit n */
            int rev_len = 0;

            while (t > 0) {
                rev[rev_len++] = (uint8_t)(t % 5u);
                t /= 5u;
            }

            seq_t sum = 0;
            for (int j = 0; j < rev_len; ++j) {
                seq_t d = (seq_t)rev[j];
                seq_t prev = cache[i - (idx_t)(j + 1)];
                sum += d * prev;
            }
            cache[i] = sum;
        }
        cache_len = (size_t)n + 1;
    }

    return cache[(size_t)n];
}
