/*
 * A235719  Squares which have one or more occurrences of exactly four
 *          different digits.
 * https://oeis.org/A235719
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   a(n) = sequence of squares whose decimal representations contain
 *            exactly four distinct digits
 * Range:     u8 none, u16 n <= 110, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   digit_counts, ensure_capacity
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include "../utils/digit_counts.h"
#include "../utils/ensure_capacity.h"

seq_t a(idx_t n)
{
    static seq_t *cache = NULL;
    static size_t cache_len = 0;          /* number of terms stored */
    static size_t cache_cap = 0;          /* allocated capacity */
    static idx_t next_k = 1;              /* next integer to test */

    while (cache_len < n) {
        seq_t sq = (seq_t)next_k * (seq_t)next_k;
        unsigned char cnt[10] = {0};
        digit_counts(sq, cnt);

        int distinct = 0;
        for (int d = 0; d < 10; ++d)
            if (cnt[d])
                ++distinct;

        if (distinct == 4) {
            /* store the square */
            ensure_capacity(cache_len + 1, &cache_cap,
                            (void **)&cache, sizeof(seq_t));
            cache[cache_len++] = sq;
        }
        ++next_k;
    }

    return cache[(size_t)(n - 1)];
}
