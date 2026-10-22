/*
 * A004825  Numbers that are the sum of at most 3 positive cubes.
 * https://oeis.org/A004825
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   sieve. Mark every x^3 + y^3 + z^3 with 0 <= x <= y <= z up to a
 *            limit, then a(n) is the n-th marked number. The limit doubles
 *            until there are at least n marked numbers
 * Range:     u8 n <= 59, u16 n <= 7793, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>

static uint64_t *vals = NULL;
static idx_t vals_len = 0;

static void build(uint64_t lim) {
    unsigned char *mark = (unsigned char *)calloc((size_t)lim + 1, 1);
    uint64_t x, y, z;
    for (x = 0; x * x * x <= lim; x++) {
        uint64_t x3 = x * x * x;
        for (y = x; x3 + y * y * y <= lim; y++) {
            uint64_t s2 = x3 + y * y * y;
            for (z = y; s2 + z * z * z <= lim; z++) {
                mark[s2 + z * z * z] = 1;
            }
        }
    }
    idx_t cnt = 0;
    uint64_t i;
    for (i = 0; i <= lim; i++) if (mark[i]) cnt++;
    free(vals);
    vals = (uint64_t *)malloc((size_t)cnt * sizeof(uint64_t));
    vals_len = 0;
    for (i = 0; i <= lim; i++) if (mark[i]) vals[vals_len++] = i;
    free(mark);
}

seq_t a(idx_t n) {
    if (vals == NULL || n > vals_len) {
        uint64_t lim = 1024;
        while (lim < (uint64_t)n * 4) lim *= 2;
        for (;;) {
            build(lim);
            if (vals_len >= n) break;
            lim *= 2;
        }
    }
    return (seq_t)vals[n - 1];
}
