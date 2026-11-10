/*
 * A080868  First differences of A080867, modulo 10.
 * https://oeis.org/A080868
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, base, easy
 * Formula:   a(n) = (A080867(n+1) - A080867(n)) mod 10, where A080867(n) = n
 *            mod 10 for n < 12, A080867(n) = (A080867(n-1) + A080867(i+1))
 *            mod 10 for n >= 12; periodic from a(1) with period 90
 * Range:     u8 n <= 1000, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Validated: standard, pass, 136 terms through n = 1000 (2026-11-07)
 * Helpers:   mem
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>
#include "../utils/mem.h"

static seq_t A080867_a(idx_t n)
{
    static seq_t *cache = NULL;
    static idx_t cache_len = 0;
    static idx_t last_occ[10];
    static idx_t second_occ[10];
    static int init = 0;

    if (!init) {
        for (int d = 0; d < 10; ++d) {
            last_occ[d] = (idx_t)-1;
            second_occ[d] = (idx_t)-1;
        }
        init = 1;
    }

    if (n + 1 > cache_len) {
        idx_t new_len = n + 1;
        if (cache_len == 0)
            cache = mem_new(seq_t, new_len);
        else
            cache = mem_resize(seq_t, cache, new_len);

        for (idx_t i = cache_len; i <= n; ++i) {
            seq_t val;
            if (i == 0) {
                val = 0;
            } else if (i < 12) {
                val = (seq_t)(i % 10);
            } else {
                seq_t prev = cache[i - 1];
                idx_t j = second_occ[(int)prev];
                if (j == (idx_t)-1) j = 0;
                seq_t next = cache[j + 1];
                val = (seq_t)((prev + next) % 10);
            }
            cache[i] = val;

            idx_t old_last = last_occ[(int)val];
            second_occ[(int)val] = old_last;
            last_occ[(int)val] = i;
        }
        cache_len = new_len;
    }

    return cache[n];
}

seq_t a(idx_t n)
{
    idx_t m = n;
    if (m >= 1) m = 1 + (m - 1) % 90;
    seq_t x = A080867_a(m);
    seq_t y = A080867_a(m + 1);
    return (seq_t)((y + 10 - x) % 10);
}
