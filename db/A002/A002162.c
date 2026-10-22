/*
 * A002162  Decimal expansion of the natural logarithm of 2.
 * https://oeis.org/A002162
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, cons
 * Formula:   log(2) = 2*Sum_{k>=0} 1/((2k+1)*3^(2k+1)) (= 2*atanh(1/3)); a(n)
 *            = floor(10^(n+1)*log(2)) mod 10
 * Range:     u8 n <= 9738, u16 n <= 9738, u32 n <= 9738, u64 n <= 9738,
 *            u128 n <= 9738, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>
#include <string.h>

#define A002162_BASE 1000000000ULL

static unsigned char *a002162_dig = NULL;
static size_t a002162_ndig = 0;

static void a002162_compute(size_t D) {
    /* fixed point: x[0] integer part, x[1..L-1] fractional limbs base 1e9 */
    size_t L = D / 9 + 5;
    uint32_t *s = (uint32_t *)calloc(L, sizeof(uint32_t));
    uint32_t *p = (uint32_t *)calloc(L, sizeof(uint32_t));
    uint32_t *t = (uint32_t *)calloc(L, sizeof(uint32_t));
    size_t i, st = 0;
    uint64_t k = 0, r, v, carry;

    /* p = 2/3 */
    p[0] = 2;
    r = 0;
    for (i = 0; i < L; i++) {
        v = r * A002162_BASE + p[i];
        p[i] = (uint32_t)(v / 3);
        r = v % 3;
    }

    for (;;) {
        uint64_t d = 2 * k + 1;
        while (st < L && p[st] == 0) st++;
        if (st == L) break;
        /* t = p / d */
        r = 0;
        for (i = st; i < L; i++) {
            v = r * A002162_BASE + p[i];
            t[i] = (uint32_t)(v / d);
            r = v % d;
        }
        /* s += t */
        carry = 0;
        for (i = L; i > st; i--) {
            v = (uint64_t)s[i - 1] + t[i - 1] + carry;
            if (v >= A002162_BASE) { s[i - 1] = (uint32_t)(v - A002162_BASE); carry = 1; }
            else { s[i - 1] = (uint32_t)v; carry = 0; }
        }
        i = st;
        while (carry && i > 0) {
            i--;
            v = (uint64_t)s[i] + carry;
            if (v >= A002162_BASE) { s[i] = (uint32_t)(v - A002162_BASE); carry = 1; }
            else { s[i] = (uint32_t)v; carry = 0; }
        }
        /* p /= 9 */
        r = 0;
        for (i = st; i < L; i++) {
            v = r * A002162_BASE + p[i];
            p[i] = (uint32_t)(v / 9);
            r = v % 9;
        }
        k++;
    }

    {
        unsigned char *buf = (unsigned char *)malloc(D);
        size_t j;
        for (j = 0; j < D; j++) {
            uint32_t limb = s[1 + j / 9];
            unsigned pos = (unsigned)(j % 9);
            unsigned e;
            for (e = 0; e < 8 - pos; e++) limb /= 10;
            buf[j] = (unsigned char)(limb % 10);
        }
        free(a002162_dig);
        a002162_dig = buf;
        a002162_ndig = D;
    }
    free(s);
    free(p);
    free(t);
}

seq_t a(idx_t n) {
    size_t idx = (size_t)n;
    if (idx >= a002162_ndig) {
        size_t D = 2 * (idx + 1);
        if (D < 1024) D = 1024;
        a002162_compute(D);
    }
    return (seq_t)a002162_dig[idx];
}
