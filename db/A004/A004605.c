/*
 * A004605  Expansion of Pi in base 6.
 * https://oeis.org/A004605
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base, cons, easy
 * Formula:   a(n) = n-th base-6 digit of Pi (a(1) = 3, the integer part),
 *            computed by Machin's formula Pi = 16*arctan(1/5) -
 *            4*arctan(1/239) in fixed-point binary, then repeatedly
 *            multiplying the fractional part by 6
 * Range:     u8 n <= 10000, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   pi6_add, pi6_atan, pi6_div, pi6_is_zero, pi6_sub
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include "../utils/pi6_add.h"
#include "../utils/pi6_atan.h"
#include "../utils/pi6_div.h"
#include "../utils/pi6_is_zero.h"
#include "../utils/pi6_sub.h"

static unsigned char *pi6_digs = NULL;
static size_t pi6_len = 0;

static void pi6_compute(size_t ndig) {
    size_t L = ndig * 13 / 160 + 6;
    uint32_t *acc = (uint32_t *)calloc(L, sizeof(uint32_t));
    uint32_t *term = (uint32_t *)calloc(L, sizeof(uint32_t));
    uint32_t *tmp = (uint32_t *)calloc(L, sizeof(uint32_t));
    unsigned char *d = (unsigned char *)malloc(ndig);
    size_t k, i;

    pi6_atan(acc, L, 16, 5, 1, term, tmp);
    pi6_atan(acc, L, 4, 239, -1, term, tmp);

    d[0] = (unsigned char)acc[0];
    acc[0] = 0;
    for (k = 1; k < ndig; k++) {
        uint64_t c = 0;
        i = L;
        while (i-- > 1) {
            uint64_t v = (uint64_t)acc[i] * 6 + c;
            acc[i] = (uint32_t)v;
            c = v >> 32;
        }
        d[k] = (unsigned char)c;
    }
    free(acc); free(term); free(tmp);
    free(pi6_digs);
    pi6_digs = d;
    pi6_len = ndig;
}

seq_t a(idx_t n) {
    if (n < 1) return 0;
    if ((size_t)n > pi6_len) {
        size_t want = (size_t)n + 16;
        if (want < 2 * pi6_len) want = 2 * pi6_len;
        if (want < 128) want = 128;
        pi6_compute(want);
    }
    return (seq_t)pi6_digs[n - 1];
}
