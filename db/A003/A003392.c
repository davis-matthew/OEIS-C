/*
 * A003392  Numbers that are the sum of 3 positive 9th powers.
 * https://oeis.org/A003392
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   search — enumerate x^9 + y^9 + z^9 with 1 <= x <= y <= z <= B,
 *            keep sums below (B+1)^9 + 2 (they are complete there), sort,
 *            deduplicate, grow B as needed
 * Range:     u8 n <= 1, u16 n <= 10, u32 n <= 274, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   a3392_p9, a3392_ucmp
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>
#include "../utils/a3392_p9.h"
#include "../utils/a3392_ucmp.h"

seq_t a(idx_t n)
{
    static uint64_t *cache = NULL;
    static idx_t cache_len = 0;
    static uint64_t B = 0;

    while (cache_len < n) {
        if (B >= 120) return 0;
        B = B ? B + B / 2 + 1 : 8;
        if (B > 120) B = 120;

        uint64_t p9[121];
        uint64_t x, y, z;
        for (x = 1; x <= B; x++) p9[x] = a3392_p9(x);
        uint64_t L = a3392_p9(B + 1) + 2;

        size_t cap = (size_t)(B * (B + 1) * (B + 2) / 6 + 1);
        uint64_t *temp = (uint64_t *)malloc(cap * sizeof(uint64_t));
        size_t count = 0;
        for (x = 1; x <= B; x++) {
            for (y = x; y <= B; y++) {
                uint64_t s1 = p9[x] + p9[y];
                if (s1 >= L) break;
                for (z = y; z <= B; z++) {
                    uint64_t s = s1 + p9[z];
                    if (s >= L) break;
                    temp[count++] = s;
                }
            }
        }
        qsort(temp, count, sizeof(uint64_t), a3392_ucmp);

        if (cache) free(cache);
        cache = (uint64_t *)malloc((count + 1) * sizeof(uint64_t));
        cache_len = 0;
        size_t i;
        for (i = 0; i < count; i++)
            if (cache_len == 0 || temp[i] != cache[cache_len - 1])
                cache[cache_len++] = temp[i];
        free(temp);
    }

    uint64_t v = cache[n - 1];
    seq_t r = (seq_t)(v >> 32);
    r = r * 65536;
    r = r * 65536;
    r += (seq_t)(v & 0xFFFFFFFFu);
    return r;
}
