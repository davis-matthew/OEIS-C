/*
 * A003370  Numbers that are the sum of 3 positive 7th powers.
 * https://oeis.org/A003370
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   search -- enumerate x^7+y^7+z^7 with 1<=x<=y<=z<=M, sort,
 *            dedupe, keep sums < (M+1)^7 (complete there), doubling M until n
 *            terms are known
 * Range:     u8 n <= 2, u16 n <= 20, u32 n <= 2143, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   a003370_cmp_u64, a003370_p7
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>
#include "../utils/a003370_cmp_u64.h"
#include "../utils/a003370_p7.h"

static uint64_t *a003370_vals = NULL;
static idx_t a003370_cnt = 0;
static uint64_t a003370_M = 0;

static void a003370_build(uint64_t M) {
    uint64_t lim = a003370_p7(M + 1);
    size_t cap = (size_t)((M * (M + 1) * (M + 2)) / 6 + 1);
    uint64_t *buf = (uint64_t *)malloc(cap * sizeof(uint64_t));
    size_t c = 0;
    uint64_t x, y, z;
    for (x = 1; x <= M; x++) {
        uint64_t x7 = a003370_p7(x);
        if (x7 >= lim) break;
        for (y = x; y <= M; y++) {
            uint64_t s2 = x7 + a003370_p7(y);
            if (s2 >= lim) break;
            for (z = y; z <= M; z++) {
                uint64_t s3 = s2 + a003370_p7(z);
                if (s3 >= lim) break;
                buf[c++] = s3;
            }
        }
    }
    qsort(buf, c, sizeof(uint64_t), a003370_cmp_u64);
    size_t w = 0, i;
    for (i = 0; i < c; i++)
        if (w == 0 || buf[i] != buf[w - 1]) buf[w++] = buf[i];
    free(a003370_vals);
    a003370_vals = buf;
    a003370_cnt = (idx_t)w;
    a003370_M = M;
}

seq_t a(idx_t n) {
    if (n < 1) return 0;
    if (a003370_M == 0) a003370_build(16);
    while (n > a003370_cnt && a003370_M < 400) {
        uint64_t M = a003370_M * 2;
        if (M > 400) M = 400;
        a003370_build(M);
    }
    if (n > a003370_cnt) return 0;
    return (seq_t)(idx_t)a003370_vals[n - 1];
}
