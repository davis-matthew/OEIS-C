/*
 * A081966  Row sums of A081964.
 * https://oeis.org/A081966
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = sum of row n of A081964 (row r = the r smallest numbers
 *            coprime to r not in earlier rows); computed by incremental
 *            search
 * Range:     u8 n <= 7, u16 n <= 41, u32 n <= 41, u64 n <= 41, u128 n <= 41,
 *            big any n
 * Validated: partial, pass, 41 terms through n = 41 (2026-11-07)
 * Helpers:   mem
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>
#include "../utils/mem.h"

static uint64_t A081966_g(uint64_t x, uint64_t y)
{
    while (y) { uint64_t t = x % y; x = y; y = t; }
    return x;
}

seq_t a(idx_t n)
{
    static seq_t *sums = NULL;
    static size_t cap = 0, len = 0;
    static uint8_t *used = NULL;
    static size_t used_cap = 0;
    static uint64_t first_free = 1;

    while (len < (size_t)n) {
        uint64_t r = (uint64_t)len + 1;
        uint64_t sel = 0;
        seq_t sum = 0;
        uint64_t cand = first_free;
        while (sel < r) {
            if ((size_t)cand >= used_cap) {
                size_t nc = used_cap == 0 ? 1024 : used_cap * 2;
                while (nc <= (size_t)cand) nc <<= 1;
                uint8_t *nu = (uint8_t *)malloc(nc);
                if (!nu) exit(1);
                for (size_t i = 0; i < used_cap; ++i) nu[i] = used[i];
                for (size_t i = used_cap; i < nc; ++i) nu[i] = 0;
                free(used);
                used = nu;
                used_cap = nc;
            }
            if (!used[cand] && A081966_g(cand, r) == 1) {
                used[cand] = 1;
                sum += (seq_t)cand;
                ++sel;
            }
            ++cand;
        }
        while ((size_t)first_free < used_cap && used[first_free]) ++first_free;
        if (len >= cap) {
            size_t nc = cap == 0 ? 64 : cap * 2;
            seq_t *ns = mem_new(seq_t, nc);
            if (sums) {
                for (size_t i = 0; i < len; ++i) ns[i] = sums[i];
                mem_free(sums);
            }
            sums = ns;
            cap = nc;
        }
        sums[len++] = sum;
    }
    return sums[n - 1];
}
