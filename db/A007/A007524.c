/*
 * A007524  Decimal expansion of log_10(2).
 * https://oeis.org/A007524
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, cons
 * Formula:   a(n) = n-th decimal digit of log_10(2) = log(2)/log(10),
 *            computed with log(2) = 2*atanh(1/3), log(10) = 3*log(2) +
 *            2*atanh(1/9) (base-1e9 fixed point), then long division
 *            log(2)/log(10) digit by digit
 * Range:     u8 n <= 9514, u16 n <= 9514, u32 n <= 9514, u64 n <= 9514,
 *            u128 n <= 9514, big any n
 * Helpers:   a7524_cmp
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include "../utils/a7524_cmp.h"

#define A7524_BASE 1000000000ULL

/* numbers are arrays x[0..len-1], x[0] integer part, rest base-1e9 fraction, MS first */

static void a7524_divsmall(uint32_t *x, int len, int start, uint64_t d)
{
    uint64_t rem = 0;
    int i;
    for (i = start; i < len; i++) {
        uint64_t cur = rem * A7524_BASE + x[i];
        x[i] = (uint32_t)(cur / d);
        rem = cur % d;
    }
}

static void a7524_mulsmall(uint32_t *x, int len, uint64_t m)
{
    uint64_t carry = 0;
    int i;
    for (i = len - 1; i >= 0; i--) {
        uint64_t cur = (uint64_t)x[i] * m + carry;
        x[i] = (uint32_t)(cur % A7524_BASE);
        carry = cur / A7524_BASE;
    }
}

static void a7524_add(uint32_t *x, const uint32_t *y, int len)
{
    uint64_t carry = 0;
    int i;
    for (i = len - 1; i >= 0; i--) {
        uint64_t cur = (uint64_t)x[i] + y[i] + carry;
        if (cur >= A7524_BASE) { x[i] = (uint32_t)(cur - A7524_BASE); carry = 1; }
        else { x[i] = (uint32_t)cur; carry = 0; }
    }
}

static void a7524_sub(uint32_t *x, const uint32_t *y, int len)
{
    int64_t borrow = 0;
    int i;
    for (i = len - 1; i >= 0; i--) {
        int64_t cur = (int64_t)x[i] - (int64_t)y[i] - borrow;
        if (cur < 0) { cur += (int64_t)A7524_BASE; borrow = 1; }
        else borrow = 0;
        x[i] = (uint32_t)cur;
    }
}

/* res = atanh(1/k) */
static void a7524_atanh_inv(uint32_t *res, int len, uint64_t k)
{
    uint32_t *t = (uint32_t *)calloc((size_t)len, sizeof(uint32_t));
    uint32_t *u = (uint32_t *)calloc((size_t)len, sizeof(uint32_t));
    uint64_t j = 1;
    int lead = 0;
    t[0] = 1;
    a7524_divsmall(t, len, 0, k);
    memcpy(res, t, (size_t)len * sizeof(uint32_t));
    for (;;) {
        a7524_divsmall(t, len, lead, k * k);
        while (lead < len && t[lead] == 0) lead++;
        if (lead >= len) break;
        memset(u, 0, (size_t)len * sizeof(uint32_t));
        memcpy(u + lead, t + lead, (size_t)(len - lead) * sizeof(uint32_t));
        a7524_divsmall(u, len, lead, 2 * j + 1);
        a7524_add(res, u, len);
        j++;
    }
    free(t);
    free(u);
}

static unsigned char *a7524_digits = NULL;
static idx_t a7524_count = 0;

static void a7524_compute(idx_t D)
{
    int len = (int)((D + 60) / 9) + 3;
    uint32_t *l2 = (uint32_t *)calloc((size_t)len, sizeof(uint32_t));
    uint32_t *l10 = (uint32_t *)calloc((size_t)len, sizeof(uint32_t));
    uint32_t *tmp = (uint32_t *)calloc((size_t)len, sizeof(uint32_t));
    uint32_t *r = (uint32_t *)calloc((size_t)len, sizeof(uint32_t));
    idx_t i;

    a7524_atanh_inv(l2, len, 3);
    a7524_mulsmall(l2, len, 2);              /* log 2 */
    memcpy(l10, l2, (size_t)len * sizeof(uint32_t));
    a7524_mulsmall(l10, len, 3);             /* 3 log 2 */
    a7524_atanh_inv(tmp, len, 9);
    a7524_mulsmall(tmp, len, 2);             /* log(5/4) */
    a7524_add(l10, tmp, len);                /* log 10 */

    free(a7524_digits);
    a7524_digits = (unsigned char *)malloc((size_t)D);
    memcpy(r, l2, (size_t)len * sizeof(uint32_t));
    for (i = 0; i < D; i++) {
        unsigned char d = 0;
        a7524_mulsmall(r, len, 10);
        while (a7524_cmp(r, l10, len) >= 0) {
            a7524_sub(r, l10, len);
            d++;
        }
        a7524_digits[i] = d;
    }
    a7524_count = D;
    free(l2);
    free(l10);
    free(tmp);
    free(r);
}

seq_t a(idx_t n)
{
    if (n >= a7524_count) {
        idx_t D = n + 1;
        if (D < 2 * a7524_count) D = 2 * a7524_count;
        if (D < 128) D = 128;
        a7524_compute(D);
    }
    return (seq_t)a7524_digits[n];
}
