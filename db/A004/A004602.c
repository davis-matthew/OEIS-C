/*
 * A004602  Expansion of Pi in base 3.
 * https://oeis.org/A004602
 *
 * Offset:    2    (a(2) is the first term)
 * Keywords:  nonn, base, cons, easy
 * Formula:   a(n) = floor(Pi*3^(n-3)) - 3*floor(Pi*3^(n-4)), n>1; Pi computed
 *            exactly in fixed point via Machin: Pi = 16*atan(1/5) -
 *            4*atan(1/239)
 * Range:     u8 n <= 10000, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   a004602_add, a004602_atan, a004602_div, a004602_iszero,
 *            a004602_sub
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include "../utils/a004602_add.h"
#include "../utils/a004602_atan.h"
#include "../utils/a004602_div.h"
#include "../utils/a004602_iszero.h"
#include "../utils/a004602_sub.h"

static unsigned char *a004602_dig = NULL;
static idx_t a004602_have = 0;

static void a004602_compute(idx_t M) {
    size_t L = (size_t)(M * 1585 / 32000) + 4;
    uint32_t *s = (uint32_t *)calloc(L + 1, sizeof(uint32_t));
    uint32_t *t = (uint32_t *)calloc(L + 1, sizeof(uint32_t));
    uint32_t *q = (uint32_t *)calloc(L + 1, sizeof(uint32_t));
    unsigned char *d = (unsigned char *)calloc((size_t)M + 4, 1);
    idx_t i;
    size_t j;

    a004602_atan(s, t, q, L, 16, 5, 1);
    a004602_atan(s, t, q, L, 4, 239, 0);

    d[2] = (unsigned char)((s[0] / 3) % 3);
    d[3] = (unsigned char)(s[0] % 3);
    s[0] = 0;
    for (i = 0; i < M; i++) {
        uint64_t c = 0;
        j = L + 1;
        while (j-- > 1) {
            uint64_t v = (uint64_t)s[j] * 3 + c;
            s[j] = (uint32_t)v;
            c = v >> 32;
        }
        d[4 + i] = (unsigned char)c;
    }
    free(s); free(t); free(q);
    free(a004602_dig);
    a004602_dig = d;
    a004602_have = M + 4;
}

seq_t a(idx_t n) {
    if (n >= a004602_have) {
        idx_t M = n + 16;
        if (M < 2 * a004602_have) M = 2 * a004602_have;
        if (M < 64) M = 64;
        a004602_compute(M);
    }
    return (seq_t)a004602_dig[n];
}
