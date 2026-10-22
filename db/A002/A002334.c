/*
 * A002334  Least positive integer x such that prime A038873(n) = x^2 - 2y^2
 *          for some y.
 * https://oeis.org/A002334
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   search: p = n-th prime with p = 2 or p == 1,7 (mod 8) (A038873);
 *            a(n) = least x > 0 with x^2 - p = 2*y^2 for some y
 * Range:     u8 n <= 1793, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   a2334_isqrt
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include "../utils/a2334_isqrt.h"

static uint64_t *a2334_pr = NULL;
static uint64_t a2334_cnt = 0;

static void a2334_build(uint64_t need) {
    uint64_t lim = 1024;
    while (a2334_cnt < need) {
        unsigned char *c = (unsigned char *)malloc((size_t)(lim + 1));
        uint64_t i, j, k = 0;
        memset(c, 0, (size_t)(lim + 1));
        for (i = 2; i * i <= lim; i++)
            if (!c[i])
                for (j = i * i; j <= lim; j += i) c[j] = 1;
        for (i = 2; i <= lim; i++)
            if (!c[i] && (i == 2 || i % 8 == 1 || i % 8 == 7)) k++;
        if (k >= need) {
            free(a2334_pr);
            a2334_pr = (uint64_t *)malloc((size_t)k * sizeof(uint64_t));
            k = 0;
            for (i = 2; i <= lim; i++)
                if (!c[i] && (i == 2 || i % 8 == 1 || i % 8 == 7)) a2334_pr[k++] = i;
            a2334_cnt = k;
        }
        free(c);
        lim *= 2;
    }
}

seq_t a(idx_t n) {
    uint64_t p, x;
    if (n < 1) return 0;
    if (a2334_cnt < (uint64_t)n) a2334_build((uint64_t)n);
    p = a2334_pr[n - 1];
    for (x = a2334_isqrt(p) + 1;; x++) {
        uint64_t d = x * x - p, h, y;
        if (d & 1) continue;
        h = d / 2;
        y = a2334_isqrt(h);
        if (y * y == h) return (seq_t)x;
    }
}
