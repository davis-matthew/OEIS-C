/*
 * A003348  Numbers that are the sum of 3 positive 5th powers.
 * https://oeis.org/A003348
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   search - list all i^5 + j^5 + k^5 with 1 <= i <= j <= k <= L,
 *            keep only values below (L+1)^5 + 2 (that range is complete),
 *            sort, and remove duplicates. If fewer than n values remain,
 *            increase L and repeat
 * Range:     u8 n <= 5, u16 n <= 126, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   a3348_cmp, a3348_p5
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>
#include "../utils/a3348_cmp.h"
#include "../utils/a3348_p5.h"

static uint64_t *a3348_cache = NULL;
static idx_t a3348_len = 0;

static void a3348_build(idx_t n) {
    uint64_t L = 8;
    for (;;) {
        uint64_t bound = a3348_p5(L + 1) + 2;
        size_t cap = (size_t)((L + 1) * (L + 2) * (L + 3) / 6 + 1);
        uint64_t *t = (uint64_t *)malloc(cap * sizeof(uint64_t));
        size_t cnt = 0;
        for (uint64_t i = 1; i <= L; i++) {
            uint64_t i5 = a3348_p5(i);
            if (3 * i5 >= bound) break;
            for (uint64_t j = i; j <= L; j++) {
                uint64_t s2 = i5 + a3348_p5(j);
                if (s2 + a3348_p5(j) >= bound) break;
                for (uint64_t k = j; k <= L; k++) {
                    uint64_t s = s2 + a3348_p5(k);
                    if (s >= bound) break;
                    t[cnt++] = s;
                }
            }
        }
        qsort(t, cnt, sizeof(uint64_t), a3348_cmp);
        size_t u = 0;
        for (size_t i = 0; i < cnt; i++)
            if (u == 0 || t[i] != t[u - 1]) t[u++] = t[i];
        if ((idx_t)u >= n) {
            free(a3348_cache);
            a3348_cache = t;
            a3348_len = (idx_t)u;
            return;
        }
        free(t);
        L += L / 2 + 1;
    }
}

seq_t a(idx_t n) {
    if (n < 1) return 0;
    if (n > a3348_len) a3348_build(n);
    return (seq_t)a3348_cache[n - 1];
}
