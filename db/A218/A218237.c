/*
 * A218237  Hilltop maps: number of n X 3 binary arrays indicating the
 *          locations of corresponding elements not exceeded by any king-move
 *          neighbor in a random 0..3 n X 3 array.
 * https://oeis.org/A218237
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) =
 *            7*a(n-1)+7*a(n-2)+7*a(n-3)+7*a(n-4)+7*a(n-5)+7*a(n-6)+7*a(n-7)
 *            with initial values (n>=1): a(1)=7, a(2)=63, a(3)=511,
 *            a(4)=4095, a(5)=32753, a(6)=262017, a(7)=2096129
 * Range:     u8 n <= 2, u16 n <= 5, u32 n <= 10, u64 n <= 21, u128 n <= 42,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

static seq_t *cache = NULL;
static size_t cache_len = 0;

seq_t a(idx_t n)
{
    if (n == 0) return 0;   /* out‑of‑range value, unused by OEIS */

    if (cache_len < n) {
        size_t new_len = (size_t)n;
        seq_t *new_cache = (seq_t *)malloc(new_len * sizeof(seq_t));
        for (size_t i = 0; i < cache_len; ++i)
            new_cache[i] = cache[i];

        /* Initial seven terms */
        const seq_t init[7] = {
            (seq_t)7,
            (seq_t)63,
            (seq_t)511,
            (seq_t)4095,
            (seq_t)32753,
            (seq_t)262017,
            (seq_t)2096129
        };

        for (idx_t i = cache_len + 1; i <= n; ++i) {
            seq_t val;
            if (i <= 7) {
                val = init[i - 1];
            } else {
                seq_t sum = 0;
                for (idx_t k = 1; k <= 7; ++k)
                    sum += new_cache[i - k - 1];
                val = 7 * sum;
            }
            new_cache[i - 1] = val;
        }

        free(cache);
        cache = new_cache;
        cache_len = n;
    }

    return cache[n - 1];
}
