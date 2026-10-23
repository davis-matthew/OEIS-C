/*
 * A118101  Decimal representation of n-th iteration of the Rule 94 elementary
 *          cellular automaton starting with a single ON cell.
 * https://oeis.org/A118101
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, base, easy
 * Formula:   a(0)=1, a(1)=7, a(2)=27, a(3)=119, a(4)=427, a(5)=1879 a(n) =
 *            17*a(n-2) - 16*a(n-4) for n>5 (This follows from OEIS A118101
 *            which also satisfies the closed‑form formulas for even/odd n.)
 * Range:     u8 n <= 3, u16 n <= 7, u32 n <= 15, u64 n <= 31, u128 n <= 63,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stddef.h>

seq_t a(idx_t n)
{
    /* static cache for memoisation */
    static seq_t *cache = NULL;
    static size_t cache_len = 0;

    /* helper: convert idx_t to size_t (safe for tested ranges) */
    size_t nn = (size_t)n;

    /* initialise cache with the first 6 values */
    if (cache_len == 0) {
        cache_len = 6;
        cache = (seq_t *)malloc(sizeof(seq_t) * cache_len);
        if (!cache) return (seq_t)0;           /* out‑of‑memory: fall back */
        cache[0] = (seq_t)1;
        cache[1] = (seq_t)7;
        cache[2] = (seq_t)27;
        cache[3] = (seq_t)119;
        cache[4] = (seq_t)427;
        cache[5] = (seq_t)1879;
    }

    /* extend cache up to n if necessary */
    if (nn >= cache_len) {
        size_t new_len = nn + 1;
        seq_t *tmp = (seq_t *)realloc(cache, sizeof(seq_t) * new_len);
        if (!tmp) return (seq_t)0;           /* out‑of‑memory: fall back */
        cache = tmp;
        for (size_t i = cache_len; i < new_len; ++i) {
            if (i <= 5) continue;            /* already initialised */
            /* recurrence: a[i] = 17*a[i-2] - 16*a[i-4]   */
            seq_t term1 = cache[i - 2] * (seq_t)17;
            seq_t term2 = cache[i - 4] * (seq_t)16;
            cache[i] = term1 - term2;
        }
        cache_len = new_len;
    }

    return cache[nn];
}
