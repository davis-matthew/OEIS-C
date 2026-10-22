/*
 * A000965  Numerators of expansion of e.g.f. sinh(x) / sin(x) (even powers
 *          only).
 * https://oeis.org/A000965
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = numerator(b(n)), where b(n) = (2n)! [x^(2n)]
 *            sinh(x)/sin(x) satisfies (2n+1)*b(n) = 1 - Sum_{k=1..n} (-1)^k *
 *            binomial(2n+1,2k+1) * b(n-k), b(0) = 1. The code scales by M =
 *            Prod_{odd primes p <= 2n+1} p^(e_p+1), with p^e_p <= 2n+1 <
 *            p^(e_p+1), so that every M*b(j) is an integer. It then di...
 * Range:     u8 n <= 3, u16 n <= 5, u32 n <= 8, u64 n <= 14, u128 n <= 22,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stddef.h>
#include <stdlib.h>
#include <string.h>

typedef struct { uint32_t *d; size_t n, cap; } q965_bn;

static void q965_init(q965_bn *a) { a->d = NULL; a->n = 0; a->cap = 0; }

static void q965_reserve(q965_bn *a, size_t c) {
    if (c > a->cap) {
        size_t nc = c * 2 + 4;
        a->d = (uint32_t *)realloc(a->d, nc * sizeof(uint32_t));
        a->cap = nc;
    }
}

static void q965_trim(q965_bn *a) {
    while (a->n && a->d[a->n - 1] == 0) a->n--;
}

static void q965_set(q965_bn *a, uint32_t v) {
    q965_reserve(a, 1);
    a->d[0] = v;
    a->n = v ? 1 : 0;
}

static void q965_copy(q965_bn *dst, const q965_bn *s) {
    q965_reserve(dst, s->n + 1);
    if (s->n) memcpy(dst->d, s->d, s->n * sizeof(uint32_t));
    dst->n = s->n;
}

static void q965_mul_small(q965_bn *a, uint32_t m) {
    uint64_t c = 0;
    size_t i;
    for (i = 0; i < a->n; i++) {
        uint64_t t = (uint64_t)a->d[i] * m + c;
        a->d[i] = (uint32_t)t;
        c = t >> 32;
    }
    if (c) {
        q965_reserve(a, a->n + 1);
        a->d[a->n++] = (uint32_t)c;
    }
    q965_trim(a);
}

static uint32_t q965_div_small(q965_bn *a, uint32_t v) {
    uint64_t r = 0;
    size_t i = a->n;
    while (i-- > 0) {
        uint64_t cur = (r << 32) | a->d[i];
        a->d[i] = (uint32_t)(cur / v);
        r = cur % v;
    }
    q965_trim(a);
    return (uint32_t)r;
}

static uint32_t q965_mod_small(const q965_bn *a, uint32_t v) {
    uint64_t r = 0;
    size_t i = a->n;
    while (i-- > 0) {
        uint64_t cur = (r << 32) | a->d[i];
        r = cur % v;
    }
    return (uint32_t)r;
}

/* acc += x * y */
static void q965_addmul(q965_bn *acc, const q965_bn *x, const q965_bn *y) {
    size_t need, i, j;
    if (!x->n || !y->n) return;
    need = x->n + y->n;
    if (acc->n > need) need = acc->n;
    need += 1;
    q965_reserve(acc, need);
    for (i = acc->n; i < need; i++) acc->d[i] = 0;
    acc->n = need;
    for (i = 0; i < x->n; i++) {
        uint64_t carry = 0, xi = x->d[i];
        size_t k;
        if (!xi) continue;
        for (j = 0; j < y->n; j++) {
            uint64_t t = xi * y->d[j] + acc->d[i + j] + carry;
            acc->d[i + j] = (uint32_t)t;
            carry = t >> 32;
        }
        k = i + y->n;
        while (carry) {
            uint64_t t = (uint64_t)acc->d[k] + carry;
            acc->d[k] = (uint32_t)t;
            carry = t >> 32;
            k++;
        }
    }
    q965_trim(acc);
}

/* a -= b, requires a >= b */
static void q965_sub(q965_bn *a, const q965_bn *b) {
    uint64_t borrow = 0;
    size_t i;
    for (i = 0; i < a->n; i++) {
        uint64_t bi = i < b->n ? b->d[i] : 0;
        uint64_t s = bi + borrow;
        uint64_t ai = a->d[i];
        a->d[i] = (uint32_t)(ai - s);
        borrow = ai < s;
    }
    q965_trim(a);
}

seq_t a(idx_t n) {
    size_t N = (size_t)n;
    uint64_t L = 2 * (uint64_t)N + 1;
    unsigned char *comp = (unsigned char *)calloc((size_t)L + 1, 1);
    int *fe = (int *)calloc((size_t)L + 1, sizeof(int));
    q965_bn M, pos, neg, C;
    q965_bn *B;
    uint64_t p;
    size_t m, k, i;
    seq_t r;

    q965_init(&M); q965_init(&pos); q965_init(&neg); q965_init(&C);
    q965_set(&M, 1);

    for (p = 3; p <= L; p += 2) {
        uint64_t pe, q;
        int e, t;
        if (comp[p]) continue;
        for (q = p * p; q <= L; q += 2 * p) comp[q] = 1;
        pe = p; e = 1;
        while (pe * p <= L) { pe *= p; e++; }
        fe[p] = e + 1;
        for (t = 0; t < e + 1; t++) q965_mul_small(&M, (uint32_t)p);
    }

    B = (q965_bn *)malloc((N + 1) * sizeof(q965_bn));
    for (i = 0; i <= N; i++) q965_init(&B[i]);
    q965_copy(&B[0], &M);

    for (m = 1; m <= N; m++) {
        q965_copy(&pos, &M);
        neg.n = 0;
        q965_set(&C, (uint32_t)(2 * m + 1));
        for (k = 1; k <= m; k++) {
            q965_mul_small(&C, (uint32_t)(2 * m - 2 * k + 2));
            q965_div_small(&C, (uint32_t)(2 * k));
            q965_mul_small(&C, (uint32_t)(2 * m - 2 * k + 1));
            q965_div_small(&C, (uint32_t)(2 * k + 1));
            if (k & 1) q965_addmul(&pos, &C, &B[m - k]);
            else q965_addmul(&neg, &C, &B[m - k]);
        }
        q965_sub(&pos, &neg);
        q965_div_small(&pos, (uint32_t)(2 * m + 1));
        q965_copy(&B[m], &pos);
    }

    for (p = 3; p <= L; p += 2) {
        int f;
        if (comp[p]) continue;
        f = fe[p];
        while (f > 0 && q965_mod_small(&B[N], (uint32_t)p) == 0) {
            q965_div_small(&B[N], (uint32_t)p);
            f--;
        }
    }

    r = 0;
    i = B[N].n;
    while (i-- > 0) {
        r = r * (seq_t)65536;
        r = r * (seq_t)65536;
        r = r + (idx_t)B[N].d[i];
    }

    for (i = 0; i <= N; i++) free(B[i].d);
    free(B);
    free(M.d); free(pos.d); free(neg.d); free(C.d);
    free(comp); free(fe);
    return r;
}
