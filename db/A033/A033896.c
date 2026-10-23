/*
 * A033896  Sort then Add, a(1) =9.
 * https://oeis.org/A033896
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base, easy
 * Formula:   a(1)=9, a(n+1)=a(n)+sort_ascending(a(n))
 * Range:     u8 n <= 6, u16 n <= 19, u32 n <= 52, u64 n <= 358,
 *            u128 n <= 1000, big any n
 * Helpers:   digit_frequencies, grow_seq
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include "../utils/digit_frequencies.h"
#include "../utils/grow_seq.h"

seq_t a(idx_t n)
{
    static seq_t *cache = NULL;          /* 1-based array of computed values */
    static idx_t cache_len = 0;          /* largest n for which cache[n] is valid */
    static size_t cache_cap = 0;         /* allocated capacity of cache */

    if (n < 1) return 0;                 /* undefined for n < 1 */

    if (cache_len == 0) {
        grow_seq(&cache, &cache_cap, 2);  /* allocate space for index 1 */
        cache[1] = (seq_t)9;
        cache_len = 1;
    }

    if (n > cache_len) {
        grow_seq(&cache, &cache_cap, (size_t)n + 1);  /* ensure space up to n */

        for (idx_t k = cache_len + 1; k <= n; ++k) {
            seq_t prev = cache[k - 1];

            unsigned int freq[10];
            digit_frequencies(prev, freq);

            seq_t add = 0;
            for (int d = 1; d <= 9; ++d) {
                unsigned int cnt = freq[d];
                for (unsigned int j = 0; j < cnt; ++j) {
                    add = add * 10 + d;
                }
            }
            cache[k] = prev + add;
        }
        cache_len = n;
    }

    return cache[n];
}
