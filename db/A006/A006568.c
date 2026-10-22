/*
 * A006568  Denominators of generalized Bernoulli numbers.
 * https://oeis.org/A006568
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, frac
 * Formula:   s(0) = 1, s(i) = -Sum_{j=0..i-1} binomial(i+2,j)*s(j) /
 *            binomial(i+2,2) (leftmost column of the inverse of the triangle
 *            binomial(i+2,j)); a(n) = denominator(s(n)). Exact rationals are
 *            held as integers E(j) = s(j)*L over a common denominator L that
 *            is kept equal to the lcm of all the denominators...
 * Range:     u8 n <= 3, u16 n <= 8, u32 n <= 17, u64 n <= 27, u128 n <= 27,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <string.h>
#include <stdint.h>

typedef struct {
    uint32_t *d;
    int len;
    int cap;
    int neg;
} bn;

static void bn_reserve(bn *a, int cap) {
    if (a->cap < cap) {
        int nc = cap * 2 > 4 ? cap * 2 : 4;
        a->d = (uint32_t *)realloc(a->d, (size_t)nc * sizeof(uint32_t));
        a->cap = nc;
    }
}

static void bn_trim(bn *a) {
    while (a->len > 0 && a->d[a->len - 1] == 0) a->len--;
}

static void bn_set_u(bn *a, uint32_t v) {
    bn_reserve(a, 1);
    a->d[0] = v;
    a->len = v ? 1 : 0;
    a->neg = 0;
}

static void bn_copy(bn *dst, const bn *src) {
    bn_reserve(dst, src->len + 1);
    if (src->len) memcpy(dst->d, src->d, (size_t)src->len * sizeof(uint32_t));
    dst->len = src->len;
    dst->neg = src->neg;
}

static void bn_mul_small(bn *a, uint32_t m) {
    uint64_t c = 0;
    int i;
    if (m == 0) { a->len = 0; return; }
    for (i = 0; i < a->len; i++) {
        uint64_t t = (uint64_t)a->d[i] * m + c;
        a->d[i] = (uint32_t)t;
        c = t >> 32;
    }
    if (c) {
        bn_reserve(a, a->len + 1);
        a->d[a->len++] = (uint32_t)c;
    }
}

static uint32_t bn_mod_small(const bn *a, uint32_t m) {
    uint64_t r = 0;
    int i;
    for (i = a->len - 1; i >= 0; i--) r = ((r << 32) | a->d[i]) % m;
    return (uint32_t)r;
}

static void bn_div_small(bn *a, uint32_t m) {
    uint64_t r = 0;
    int i;
    for (i = a->len - 1; i >= 0; i--) {
        uint64_t cur = (r << 32) | a->d[i];
        a->d[i] = (uint32_t)(cur / m);
        r = cur % m;
    }
    bn_trim(a);
}

/* acc += x * y (magnitudes) */
static void bn_addmul(bn *acc, const bn *x, const bn *y) {
    int nl, i, j, k;
    if (x->len == 0 || y->len == 0) return;
    nl = (acc->len > x->len + y->len ? acc->len : x->len + y->len) + 1;
    bn_reserve(acc, nl);
    for (k = acc->len; k < nl; k++) acc->d[k] = 0;
    for (i = 0; i < x->len; i++) {
        uint64_t carry = 0, xi = x->d[i];
        for (j = 0; j < y->len; j++) {
            uint64_t t = xi * y->d[j] + acc->d[i + j] + carry;
            acc->d[i + j] = (uint32_t)t;
            carry = t >> 32;
        }
        k = i + y->len;
        while (carry) {
            uint64_t t = (uint64_t)acc->d[k] + carry;
            acc->d[k] = (uint32_t)t;
            carry = t >> 32;
            k++;
        }
    }
    acc->len = nl;
    bn_trim(acc);
}

static int bn_cmp(const bn *a, const bn *b) {
    int i;
    if (a->len != b->len) return a->len < b->len ? -1 : 1;
    for (i = a->len - 1; i >= 0; i--)
        if (a->d[i] != b->d[i]) return a->d[i] < b->d[i] ? -1 : 1;
    return 0;
}

/* a -= b, requires |a| >= |b| */
static void bn_sub_in(bn *a, const bn *b) {
    uint64_t borrow = 0;
    int k;
    for (k = 0; k < a->len; k++) {
        uint64_t bk = k < b->len ? b->d[k] : 0;
        uint64_t t = (uint64_t)a->d[k] - bk - borrow;
        a->d[k] = (uint32_t)t;
        borrow = (t >> 63) & 1;
    }
    bn_trim(a);
}

static bn *E_arr = NULL, *den_arr = NULL;
static int cap_arr = 0, done_cnt = 0;
static int *dexp = NULL;
static int dcap = 0;
static bn B_, pos_, neg_, tmp_;

static int add_factors(uint32_t x, uint32_t *ps, int cnt) {
    uint32_t p;
    for (p = 2; (uint64_t)p * p <= x; p++) {
        if (x % p == 0) {
            int k, seen = 0;
            while (x % p == 0) { x /= p; dexp[p]++; }
            for (k = 0; k < cnt; k++) if (ps[k] == p) seen = 1;
            if (!seen) ps[cnt++] = p;
        }
    }
    if (x > 1) {
        int k, seen = 0;
        dexp[x]++;
        for (k = 0; k < cnt; k++) if (ps[k] == x) seen = 1;
        if (!seen) ps[cnt++] = x;
    }
    return cnt;
}

static void step(int i) {
    uint32_t m = (uint32_t)i + 2;
    uint32_t a1 = m, a2 = m - 1;
    uint32_t ps[64];
    int cnt = 0, j, k;
    uint32_t p;
    bn *e = &E_arr[i];

    bn_set_u(&B_, 1);
    pos_.len = 0;
    neg_.len = 0;
    for (j = 0; j < i; j++) {
        if (E_arr[j].len) bn_addmul(E_arr[j].neg ? &neg_ : &pos_, &B_, &E_arr[j]);
        bn_mul_small(&B_, m - (uint32_t)j);
        bn_div_small(&B_, (uint32_t)j + 1);
    }
    if (bn_cmp(&pos_, &neg_) >= 0) {
        bn_sub_in(&pos_, &neg_);
        bn_copy(e, &pos_);
        e->neg = 1;
    } else {
        bn_sub_in(&neg_, &pos_);
        bn_copy(e, &neg_);
        e->neg = 0;
    }
    if (e->len == 0) e->neg = 0;

    if (a1 % 2 == 0) a1 /= 2; else a2 /= 2;
    for (j = 0; j < i; j++) {
        bn_mul_small(&E_arr[j], a1);
        bn_mul_small(&E_arr[j], a2);
    }
    cnt = add_factors(a1, ps, cnt);
    cnt = add_factors(a2, ps, cnt);

    for (k = 0; k < cnt; k++) {
        p = ps[k];
        while (dexp[p] > 0) {
            int ok = 1;
            for (j = 0; j <= i && ok; j++)
                if (E_arr[j].len && bn_mod_small(&E_arr[j], p) != 0) ok = 0;
            if (!ok) break;
            for (j = 0; j <= i; j++)
                if (E_arr[j].len) bn_div_small(&E_arr[j], p);
            dexp[p]--;
        }
    }

    bn_set_u(&den_arr[i], 1);
    bn_copy(&tmp_, e);
    for (p = 2; p <= m; p++) {
        int v = 0, r;
        if (dexp[p] <= 0) continue;
        if (tmp_.len == 0) {
            v = dexp[p];
        } else {
            while (v < dexp[p] && bn_mod_small(&tmp_, p) == 0) {
                bn_div_small(&tmp_, p);
                v++;
            }
        }
        for (r = v; r < dexp[p]; r++) bn_mul_small(&den_arr[i], p);
    }
}

static void ensure(int N) {
    int i;
    if (N + 1 > cap_arr) {
        int nc = N + 1 > 2 * cap_arr ? N + 1 : 2 * cap_arr;
        E_arr = (bn *)realloc(E_arr, (size_t)nc * sizeof(bn));
        den_arr = (bn *)realloc(den_arr, (size_t)nc * sizeof(bn));
        memset(E_arr + cap_arr, 0, (size_t)(nc - cap_arr) * sizeof(bn));
        memset(den_arr + cap_arr, 0, (size_t)(nc - cap_arr) * sizeof(bn));
        cap_arr = nc;
    }
    if (N + 3 > dcap) {
        int nc = N + 3 > 2 * dcap ? N + 3 : 2 * dcap;
        dexp = (int *)realloc(dexp, (size_t)nc * sizeof(int));
        memset(dexp + dcap, 0, (size_t)(nc - dcap) * sizeof(int));
        dcap = nc;
    }
    if (done_cnt == 0) {
        bn_set_u(&E_arr[0], 1);
        bn_set_u(&den_arr[0], 1);
        done_cnt = 1;
    }
    for (i = done_cnt; i <= N; i++) {
        step(i);
        done_cnt = i + 1;
    }
}

seq_t a(idx_t n) {
    int N = (int)n;
    int k, sh;
    seq_t r = 0;
    const bn *d;
    ensure(N);
    d = &den_arr[N];
    for (k = d->len - 1; k >= 0; k--) {
        for (sh = 24; sh >= 0; sh -= 8) {
            r = r * 256;
            r += (seq_t)((d->d[k] >> sh) & 255u);
        }
    }
    return r;
}
