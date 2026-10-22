/*
 * A000087  Number of unrooted nonseparable planar maps with n edges and a
 *          distinguished face.
 * https://oeis.org/A000087
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = (1/3n)[(n+2)binomial(3n, n)/((3n-2)(3n-1)) + Sum_{0<k<n,
 *            k|n}phi(n/k)binomial(3k, k)] + q(n), q(n) = 0 for even n, q(n) =
 *            2(n+1)binomial(3(n+1)/2, (n+1)/2)/(3(3n-1)(3n+1)) for odd n.
 *            With R(n) = 2(3n-3)!/(n!(2n-1)!) = 2*binomial(3n-3,
 *            n-1)/(n(2n-1)) and m = (n+1)/2, this becomes 12n*a(n) =...
 * Range:     u8 n <= 7, u16 n <= 10, u32 n <= 17, u64 n <= 29, u128 n <= 53,
 *            big any n
 * Helpers:   a87_phi
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include "../utils/a87_phi.h"

typedef struct {
    uint32_t *d;
    size_t len;
    size_t cap;
} a87_bn;

static void a87_init(a87_bn *x, size_t cap, uint32_t v)
{
    x->d = (uint32_t *)calloc(cap, sizeof(uint32_t));
    x->cap = cap;
    x->d[0] = v;
    x->len = v ? 1 : 0;
}

static void a87_set(a87_bn *x, uint32_t v)
{
    memset(x->d, 0, x->cap * sizeof(uint32_t));
    x->d[0] = v;
    x->len = v ? 1 : 0;
}

static void a87_mul(a87_bn *x, uint32_t m)
{
    uint64_t c = 0;
    size_t i;
    if (m == 0) { x->len = 0; return; }
    for (i = 0; i < x->len; i++) {
        uint64_t t = (uint64_t)x->d[i] * m + c;
        x->d[i] = (uint32_t)t;
        c = t >> 32;
    }
    if (c) x->d[x->len++] = (uint32_t)c;
}

static uint32_t a87_div(a87_bn *x, uint32_t m)
{
    uint64_t r = 0;
    size_t i = x->len;
    while (i > 0) {
        uint64_t cur;
        i--;
        cur = (r << 32) | x->d[i];
        x->d[i] = (uint32_t)(cur / m);
        r = cur % m;
    }
    while (x->len > 0 && x->d[x->len - 1] == 0) x->len--;
    return (uint32_t)r;
}

static void a87_add(a87_bn *x, const a87_bn *y)
{
    uint64_t c = 0;
    size_t n = x->len > y->len ? x->len : y->len;
    size_t i;
    for (i = 0; i < n; i++) {
        uint64_t t = c;
        if (i < x->len) t += x->d[i];
        if (i < y->len) t += y->d[i];
        x->d[i] = (uint32_t)t;
        c = t >> 32;
    }
    x->len = n;
    if (c) x->d[x->len++] = (uint32_t)c;
}

/* R(n) = 2*binomial(3n-3, n-1)/(n(2n-1)) */
static void a87_rooted(a87_bn *x, uint32_t n)
{
    uint32_t i;
    a87_set(x, 1);
    for (i = 1; i < n; i++) {
        a87_mul(x, 2 * n - 2 + i);
        a87_div(x, i);
    }
    a87_mul(x, 2);
    a87_div(x, n);
    a87_div(x, 2 * n - 1);
}

seq_t a(idx_t n)
{
    uint32_t nn = (uint32_t)n;
    size_t cap = (size_t)(3 * (uint64_t)nn) / 32 + 16;
    a87_bn N, S, t;
    uint32_t k;
    seq_t r = 0;
    size_t i;

    a87_init(&N, cap, 0);
    a87_init(&S, cap, 0);
    a87_init(&t, cap, 0);

    /* 3(n+2)R(n) */
    a87_rooted(&N, nn);
    a87_mul(&N, 3);
    a87_mul(&N, nn + 2);

    /* 4 * Sum phi(n/k) binomial(3k,k) */
    for (k = 1; k < nn; k++) {
        uint32_t j;
        if (nn % k != 0) continue;
        a87_set(&t, 1);
        for (j = 1; j <= k; j++) {
            a87_mul(&t, 2 * k + j);
            a87_div(&t, j);
        }
        a87_mul(&t, (uint32_t)a87_phi(nn / k));
        a87_add(&S, &t);
    }
    a87_mul(&S, 4);
    a87_add(&N, &S);

    /* odd n: 3n*m*R(m), m = (n+1)/2 */
    if (nn & 1) {
        uint32_t m = (nn + 1) / 2;
        a87_rooted(&t, m);
        a87_mul(&t, 3);
        a87_mul(&t, nn);
        a87_mul(&t, m);
        a87_add(&N, &t);
    }

    a87_div(&N, 12);
    a87_div(&N, nn);

    i = N.len;
    while (i > 0) {
        i--;
        r = r * 65536u + (seq_t)(N.d[i] >> 16);
        r = r * 65536u + (seq_t)(N.d[i] & 0xffffu);
    }

    free(N.d);
    free(S.d);
    free(t.d);
    return r;
}
