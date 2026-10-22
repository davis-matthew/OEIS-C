/*
 * A002945  Continued fraction for cube root of 2.
 * https://oeis.org/A002945
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  cofr, nonn
 * Formula:   a(n) is the n-th partial quotient of 2^(1/3). It is computed
 *            with Lagrange's continued-fraction method: start from P(x) = x^3
 *            - 2. a = floor of P's unique real root > 0 (found by doubling
 *            and bisection on the sign of P(t)). Then replace P(x) by y^3 *
 *            P(a + 1/y) and repeat. Exact multi-precision i...
 * Range:     u8 n <= 34, u16 n <= 9088, u32 n <= 9088, u64 n <= 9088,
 *            u128 n <= 9088, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>
#include <string.h>

typedef struct { uint32_t *d; size_t n, cap; int neg; } cr2_t;

static void cr2_res(cr2_t *x, size_t n) {
    if (n > x->cap) {
        size_t c = x->cap ? x->cap * 2 : 8;
        while (c < n) c *= 2;
        x->d = (uint32_t *)realloc(x->d, c * sizeof(uint32_t));
        x->cap = c;
    }
}

static void cr2_trim(cr2_t *x) {
    while (x->n && x->d[x->n - 1] == 0) x->n--;
    if (!x->n) x->neg = 0;
}

static void cr2_copy(cr2_t *x, const cr2_t *y) {
    cr2_res(x, y->n + 1);
    if (y->n) memcpy(x->d, y->d, y->n * sizeof(uint32_t));
    x->n = y->n;
    x->neg = y->neg;
}

static void cr2_setsmall(cr2_t *x, uint32_t v, int neg) {
    cr2_res(x, 1);
    x->d[0] = v;
    x->n = 1;
    x->neg = neg;
    cr2_trim(x);
}

static void cr2_mul(cr2_t *x, uint32_t m) {
    uint64_t carry = 0;
    size_t i;
    for (i = 0; i < x->n; i++) {
        uint64_t t = (uint64_t)x->d[i] * m + carry;
        x->d[i] = (uint32_t)t;
        carry = t >> 32;
    }
    if (carry) {
        cr2_res(x, x->n + 1);
        x->d[x->n++] = (uint32_t)carry;
    }
    cr2_trim(x);
}

static int cr2_cmpmag(const cr2_t *a, const cr2_t *b) {
    size_t i;
    if (a->n != b->n) return a->n < b->n ? -1 : 1;
    for (i = a->n; i-- > 0;) {
        if (a->d[i] != b->d[i]) return a->d[i] < b->d[i] ? -1 : 1;
    }
    return 0;
}

static void cr2_add(cr2_t *x, const cr2_t *y) {
    size_t i;
    if (y->n == 0) return;
    if (x->n == 0 || x->neg == y->neg) {
        size_t n = x->n > y->n ? x->n : y->n;
        uint64_t carry = 0;
        if (x->n == 0) x->neg = y->neg;
        cr2_res(x, n + 1);
        for (i = x->n; i < n; i++) x->d[i] = 0;
        for (i = 0; i < n; i++) {
            uint64_t t = (uint64_t)x->d[i] + (i < y->n ? y->d[i] : 0) + carry;
            x->d[i] = (uint32_t)t;
            carry = t >> 32;
        }
        x->n = n;
        if (carry) x->d[x->n++] = 1;
    } else {
        int c = cr2_cmpmag(x, y);
        uint64_t borrow = 0;
        if (c == 0) { x->n = 0; x->neg = 0; return; }
        if (c > 0) {
            for (i = 0; i < x->n; i++) {
                uint64_t t = (uint64_t)x->d[i] - (i < y->n ? y->d[i] : 0) - borrow;
                x->d[i] = (uint32_t)t;
                borrow = t >> 63;
            }
        } else {
            cr2_res(x, y->n + 1);
            for (i = x->n; i < y->n; i++) x->d[i] = 0;
            for (i = 0; i < y->n; i++) {
                uint64_t t = (uint64_t)y->d[i] - x->d[i] - borrow;
                x->d[i] = (uint32_t)t;
                borrow = t >> 63;
            }
            x->n = y->n;
            x->neg = y->neg;
        }
    }
    cr2_trim(x);
}

static int cr2_sign(const cr2_t *x) {
    return x->n == 0 ? 0 : (x->neg ? -1 : 1);
}

static cr2_t cr2_c[4];
static cr2_t cr2_tmp;
static int cr2_init = 0;
static uint64_t *cr2_terms = NULL;
static size_t cr2_count = 0, cr2_cap = 0;

static int cr2_eval_sign(uint32_t t) {
    int j;
    cr2_copy(&cr2_tmp, &cr2_c[0]);
    for (j = 1; j < 4; j++) {
        cr2_mul(&cr2_tmp, t);
        cr2_add(&cr2_tmp, &cr2_c[j]);
    }
    return cr2_sign(&cr2_tmp);
}

static void cr2_step(void) {
    int sA = cr2_sign(&cr2_c[0]);
    uint64_t lo = 1, hi = 2, a;
    int i, j;
    cr2_t sw;
    while (cr2_eval_sign((uint32_t)hi) != sA) { lo = hi; hi *= 2; }
    while (hi - lo > 1) {
        uint64_t mid = lo + (hi - lo) / 2;
        if (cr2_eval_sign((uint32_t)mid) == sA) hi = mid; else lo = mid;
    }
    a = lo;
    for (i = 0; i < 3; i++) {
        for (j = 1; j <= 3 - i; j++) {
            cr2_copy(&cr2_tmp, &cr2_c[j - 1]);
            cr2_mul(&cr2_tmp, (uint32_t)a);
            cr2_add(&cr2_c[j], &cr2_tmp);
        }
    }
    sw = cr2_c[0]; cr2_c[0] = cr2_c[3]; cr2_c[3] = sw;
    sw = cr2_c[1]; cr2_c[1] = cr2_c[2]; cr2_c[2] = sw;
    if (cr2_count >= cr2_cap) {
        size_t nc = cr2_cap ? cr2_cap * 2 : 64;
        cr2_terms = (uint64_t *)realloc(cr2_terms, nc * sizeof(uint64_t));
        cr2_cap = nc;
    }
    cr2_terms[cr2_count++] = a;
}

seq_t a(idx_t n) {
    if (!cr2_init) {
        int i;
        for (i = 0; i < 4; i++) {
            cr2_c[i].d = NULL; cr2_c[i].n = 0; cr2_c[i].cap = 0; cr2_c[i].neg = 0;
        }
        cr2_tmp.d = NULL; cr2_tmp.n = 0; cr2_tmp.cap = 0; cr2_tmp.neg = 0;
        cr2_setsmall(&cr2_c[0], 1, 0);
        cr2_setsmall(&cr2_c[1], 0, 0);
        cr2_setsmall(&cr2_c[2], 0, 0);
        cr2_setsmall(&cr2_c[3], 2, 1);
        cr2_init = 1;
    }
    while ((idx_t)cr2_count <= n) cr2_step();
    return (seq_t)cr2_terms[n];
}
