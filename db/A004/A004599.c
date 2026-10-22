/*
 * A004599  Expansion of e in base 8.
 * https://oeis.org/A004599
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base, cons
 * Formula:   a(1) = 2, a(n) = (n-1)-th base-8 fractional digit of e, computed
 *            by the Rabinowitz–Wagon spigot on e - 2 = Sum_{k>=2} 1/k!
 * Range:     u8 n <= 10000, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <string.h>
#include <stdint.h>

static unsigned char *e8_dig = NULL;
static size_t e8_cnt = 0;

static void e8_compute(size_t want) {
    /* choose m with m! > 8^(want+20), using floor(log2 i) as a safe underestimate */
    size_t needbits = 3 * (want + 20);
    size_t bits = 0, m = 1;
    while (bits <= needbits) {
        m++;
        size_t t = m, b = 0;
        while (t > 1) { t >>= 1; b++; }
        bits += b;
    }
    uint32_t *r = (uint32_t *)malloc((m + 1) * sizeof(uint32_t));
    size_t i, d;
    for (i = 0; i <= m; i++) r[i] = 1;
    unsigned char *nd = (unsigned char *)malloc(want + 1);
    for (d = 0; d < want; d++) {
        uint32_t carry = 0;
        for (i = m; i >= 2; i--) {
            uint32_t x = r[i] * 8u + carry;
            r[i] = x % (uint32_t)i;
            carry = x / (uint32_t)i;
        }
        nd[d] = (unsigned char)carry;
    }
    free(r);
    free(e8_dig);
    e8_dig = nd;
    e8_cnt = want;
}

seq_t a(idx_t n) {
    if (n <= 1) return (seq_t)2;
    size_t k = (size_t)(n - 1); /* 1-based fractional digit index */
    if (k > e8_cnt) {
        size_t want = k + 16;
        if (want < 2 * e8_cnt) want = 2 * e8_cnt;
        if (want < 64) want = 64;
        e8_compute(want);
    }
    return (seq_t)e8_dig[k - 1];
}
