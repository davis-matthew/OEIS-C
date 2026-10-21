/*
 * A091855  Odious numbers (see A000069) in A003159.
 * https://oeis.org/A091855
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = nth integer n>0 such that (number of 1‑bits in binary
 *            expansion of n) is odd and (number of trailing zero bits of n)
 *            is even
 * Range:     u8 n <= 85, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>

/* External linkage as required. */
seq_t a(idx_t n)
{
    if (n == 0) return 0;                 /* offset starts at 1 */

    /* Cache of previously computed terms. */
    static seq_t *cache  = NULL;          /* array of terms */
    static idx_t  cache_len = 0;          /* number of terms stored */
    static idx_t  cache_cap = 0;          /* current capacity of cache */
    static idx_t  tested    = 1;          /* next integer to test */

    /* Grow cache when needed. */
    if (cache_len == 0) {
        cache_cap = 64;
        cache = (seq_t *)malloc(cache_cap * sizeof(seq_t));
        if (!cache) return 0;            /* allocation failure: return 0 */
    }

    while (cache_len < n) {
        /* Find next qualifying integer. */
        while (1) {
            uint64_t m = tested++;
            uint64_t pop = __builtin_popcountll(m);
            uint64_t tz  = __builtin_ctzll(m);
            if ((pop & 1) && !(tz & 1)) { /* odd popcount & even trailing zeros */
                if (cache_len == cache_cap) {
                    cache_cap *= 2;
                    seq_t *tmp = (seq_t *)realloc(cache, cache_cap * sizeof(seq_t));
                    if (!tmp) return 0; /* allocation failure */
                    cache = tmp;
                }
                cache[cache_len++] = (seq_t)m;
                break;
            }
        }
    }

    return cache[n - 1]; /* a(1) is the first term */
}
