/*
 * A002551  Numerators of coefficients in Taylor series expansion of
 *          log(1+x)^2/sqrt(1+x).
 * https://oeis.org/A002551
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  sign, frac
 * Formula:   a(n) = numerator of c(n), where c(n) = [x^n]
 *            log(1+x)^2/sqrt(1+x) = d^2/ds^2 binomial(s,n) at s = -1/2 =
 *            (-1)^n * C(2n,n)/4^n * 4 * (O1^2 - O2), O1 = Sum_{k=0..n-1}
 *            1/(2k+1), O2 = Sum_{k=0..n-1} 1/(2k+1)^2. With D =
 *            lcm(1,3,...,2n-1), S1 = Sum D/(2k+1), S2 = Sum D^2/(2k+1)^2:
 *            c(n) = (-1)^n * 4*C(...
 * Range:     i8 n <= 5, i16 n <= 7, i32 n <= 11, i64 n <= 19, i128 n <= 21,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>
#include <string.h>

typedef struct { uint32_t *d; size_t n, cap; } q2551_t;

static void q2551_reserve(q2551_t *x, size_t c) {
    if (c < 1) c = 1;
    if (c > x->cap) {
        x->d = (uint32_t *)realloc(x->d, c * sizeof(uint32_t));
        memset(x->d + x->cap, 0, (c - x->cap) * sizeof(uint32_t));
        x->cap = c;
    }
}

static void q2551_trim(q2551_t *x) {
    while (x->n > 0 && x->d[x->n - 1] == 0) x->n--;
}

static void q2551_set(q2551_t *x, uint32_t v) {
    q2551_reserve(x, 1);
    x->d[0] = v;
    x->n = v ? 1 : 0;
}

static void q2551_muls(q2551_t *x, uint32_t m) {
    uint64_t carry = 0;
    size_t i;
    for (i = 0; i < x->n; i++) {
        uint64_t t = (uint64_t)x->d[i] * m + carry;
        x->d[i] = (uint32_t)t;
        carry = t >> 32;
    }
    if (carry) {
        q2551_reserve(x, x->n + 1);
        x->d[x->n++] = (uint32_t)carry;
    }
    q2551_trim(x);
}

/* q = x / m (q may be x); returns remainder */
static uint32_t q2551_divs(q2551_t *q, const q2551_t *x, uint32_t m) {
    uint64_t r = 0;
    size_t i, len = x->n;
    if (q != x) q2551_reserve(q, len);
    for (i = len; i-- > 0;) {
        uint64_t cur = (r << 32) | x->d[i];
        q->d[i] = (uint32_t)(cur / m);
        r = cur % m;
    }
    q->n = len;
    q2551_trim(q);
    return (uint32_t)r;
}

static uint32_t q2551_mods(const q2551_t *x, uint32_t m) {
    uint64_t r = 0;
    size_t i;
    for (i = x->n; i-- > 0;) r = ((r << 32) | x->d[i]) % m;
    return (uint32_t)r;
}

static void q2551_add(q2551_t *x, const q2551_t *y) {
    size_t mx = x->n > y->n ? x->n : y->n, i;
    uint64_t carry = 0;
    q2551_reserve(x, mx + 1);
    for (i = 0; i < mx; i++) {
        uint64_t t = (uint64_t)(i < x->n ? x->d[i] : 0)
                   + (uint64_t)(i < y->n ? y->d[i] : 0) + carry;
        x->d[i] = (uint32_t)t;
        carry = t >> 32;
    }
    x->d[mx] = (uint32_t)carry;
    x->n = mx + 1;
    q2551_trim(x);
}

/* x -= y, requires x >= y */
static void q2551_sub(q2551_t *x, const q2551_t *y) {
    size_t i;
    int64_t borrow = 0;
    for (i = 0; i < x->n; i++) {
        int64_t t = (int64_t)x->d[i] - (int64_t)(i < y->n ? y->d[i] : 0) - borrow;
        if (t < 0) { t += ((int64_t)1 << 32); borrow = 1; } else borrow = 0;
        x->d[i] = (uint32_t)t;
    }
    q2551_trim(x);
}

/* r = x * y, r distinct from x and y */
static void q2551_mul(q2551_t *r, const q2551_t *x, const q2551_t *y) {
    size_t i, j, len = x->n + y->n;
    q2551_reserve(r, len);
    memset(r->d, 0, (len ? len : 1) * sizeof(uint32_t));
    for (i = 0; i < x->n; i++) {
        uint64_t carry = 0;
        for (j = 0; j < y->n; j++) {
            uint64_t t = (uint64_t)x->d[i] * y->d[j] + r->d[i + j] + carry;
            r->d[i + j] = (uint32_t)t;
            carry = t >> 32;
        }
        r->d[i + y->n] = (uint32_t)carry;
    }
    r->n = len;
    q2551_trim(r);
}

seq_t a(idx_t n) {
    uint64_t M, p, i, k;
    char *comp;
    q2551_t D = {NULL, 0, 0}, D2 = {NULL, 0, 0}, S1 = {NULL, 0, 0};
    q2551_t S2 = {NULL, 0, 0}, T = {NULL, 0, 0}, X = {NULL, 0, 0};
    q2551_t C = {NULL, 0, 0}, Num = {NULL, 0, 0};
    seq_t r = 0;
    size_t li;

    if (n < 2) return 0;

    M = 2 * (uint64_t)n - 1;
    comp = (char *)calloc(M + 1, 1);
    for (i = 2; i * i <= M; i++)
        if (!comp[i])
            for (k = i * i; k <= M; k += i) comp[k] = 1;

    /* D = lcm of odd numbers <= M */
    q2551_set(&D, 1);
    for (p = 3; p <= M; p += 2) {
        uint64_t pe;
        if (comp[p]) continue;
        for (pe = p; pe <= M; pe *= p) q2551_muls(&D, (uint32_t)p);
    }
    q2551_mul(&D2, &D, &D);

    q2551_set(&S1, 0);
    q2551_set(&S2, 0);
    for (k = 0; k < n; k++) {
        uint32_t m = (uint32_t)(2 * k + 1);
        q2551_divs(&T, &D, m);
        q2551_add(&S1, &T);
        q2551_divs(&T, &D2, m);
        q2551_divs(&T, &T, m);
        q2551_add(&S2, &T);
    }

    q2551_mul(&X, &S1, &S1);
    q2551_sub(&X, &S2);

    /* C = binomial(2n, n) */
    q2551_set(&C, 1);
    for (k = 1; k <= n; k++) {
        q2551_muls(&C, (uint32_t)(n + k));
        q2551_divs(&C, &C, (uint32_t)k);
    }

    q2551_mul(&Num, &X, &C);
    q2551_muls(&Num, 4);

    if (Num.n > 0) {
        /* strip common factors with denominator 2^(2n) * D^2 */
        uint64_t e = 2 * (uint64_t)n;
        while (e > 0 && (Num.d[0] & 1u) == 0) {
            q2551_divs(&Num, &Num, 2);
            e--;
        }
        for (p = 3; p <= M; p += 2) {
            uint64_t pe;
            if (comp[p]) continue;
            e = 0;
            for (pe = p; pe <= M; pe *= p) e += 2;
            while (e > 0 && q2551_mods(&Num, (uint32_t)p) == 0) {
                q2551_divs(&Num, &Num, (uint32_t)p);
                e--;
            }
        }

        for (li = Num.n; li-- > 0;) {
            uint32_t w = Num.d[li];
            r = (r << 16) + (seq_t)(long long)(w >> 16);
            r = (r << 16) + (seq_t)(long long)(w & 0xFFFFu);
        }
        if (n % 2 == 1) r = -r;
    }

    free(comp);
    free(D.d); free(D2.d); free(S1.d); free(S2.d);
    free(T.d); free(X.d); free(C.d); free(Num.d);
    return r;
}
