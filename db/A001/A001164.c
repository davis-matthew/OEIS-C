/*
 * A001164  Stirling's formula: denominators of asymptotic series for Gamma
 *          function.
 * https://oeis.org/A001164
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, frac, nice
 * Formula:   a(n) = denominator(h(2*n)*(2*n-1)!!), where h(0) = 1 and h(k) =
 *            (h(k-1)/k - Sum_{j=1..k-1} h(k-j)*h(j)/(j+1))/(1+1/(k+1))
 * Range:     u8 n <= 1, u16 n <= 3, u32 n <= 5, u64 n <= 9, u128 n <= 17,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>
#include <string.h>

typedef struct { uint32_t *w; size_t n, cap; int neg; } a1164_Z;

static void a1164_init(a1164_Z *x) { x->w = NULL; x->n = 0; x->cap = 0; x->neg = 0; }

static void a1164_reserve(a1164_Z *x, size_t cap) {
    if (cap > x->cap) {
        size_t nc = x->cap ? x->cap : 4;
        while (nc < cap) nc *= 2;
        x->w = (uint32_t *)realloc(x->w, nc * sizeof(uint32_t));
        x->cap = nc;
    }
}

static void a1164_norm(a1164_Z *x) {
    while (x->n && x->w[x->n - 1] == 0) x->n--;
    if (!x->n) x->neg = 0;
}

static void a1164_copy(a1164_Z *r, const a1164_Z *a) {
    a1164_reserve(r, a->n + 1);
    if (a->n) memcpy(r->w, a->w, a->n * sizeof(uint32_t));
    r->n = a->n;
    r->neg = a->neg;
}

static void a1164_muls(a1164_Z *x, uint32_t m) {
    uint64_t c = 0;
    size_t i;
    if (m == 0) { x->n = 0; x->neg = 0; return; }
    for (i = 0; i < x->n; i++) {
        uint64_t t = (uint64_t)x->w[i] * m + c;
        x->w[i] = (uint32_t)t;
        c = t >> 32;
    }
    if (c) {
        a1164_reserve(x, x->n + 1);
        x->w[x->n++] = (uint32_t)c;
    }
}

static void a1164_mul(a1164_Z *r, const a1164_Z *a, const a1164_Z *b) {
    size_t i, j;
    if (a->n == 0 || b->n == 0) { r->n = 0; r->neg = 0; return; }
    a1164_reserve(r, a->n + b->n);
    memset(r->w, 0, (a->n + b->n) * sizeof(uint32_t));
    for (i = 0; i < a->n; i++) {
        uint64_t c = 0, ai = a->w[i];
        for (j = 0; j < b->n; j++) {
            uint64_t t = ai * b->w[j] + r->w[i + j] + c;
            r->w[i + j] = (uint32_t)t;
            c = t >> 32;
        }
        r->w[i + b->n] = (uint32_t)c;
    }
    r->n = a->n + b->n;
    r->neg = a->neg ^ b->neg;
    a1164_norm(r);
}

static uint32_t a1164_mods(const a1164_Z *x, uint32_t p) {
    uint64_t r = 0;
    size_t i = x->n;
    while (i-- > 0) r = ((r << 32) | x->w[i]) % p;
    return (uint32_t)r;
}

static void a1164_divs(a1164_Z *x, uint32_t p) {
    uint64_t r = 0;
    size_t i = x->n;
    while (i-- > 0) {
        uint64_t cur = (r << 32) | x->w[i];
        x->w[i] = (uint32_t)(cur / p);
        r = cur % p;
    }
    a1164_norm(x);
}

static int a1164_cmpmag(const a1164_Z *a, const a1164_Z *b) {
    size_t i;
    if (a->n != b->n) return a->n > b->n ? 1 : -1;
    i = a->n;
    while (i-- > 0)
        if (a->w[i] != b->w[i]) return a->w[i] > b->w[i] ? 1 : -1;
    return 0;
}

static void a1164_addto(a1164_Z *acc, const a1164_Z *t) {
    size_t i;
    if (t->n == 0) return;
    if (acc->n == 0) { a1164_copy(acc, t); return; }
    if (acc->neg == t->neg) {
        size_t L = acc->n > t->n ? acc->n : t->n;
        uint64_t c = 0;
        a1164_reserve(acc, L + 1);
        for (i = acc->n; i <= L; i++) acc->w[i] = 0;
        for (i = 0; i < L; i++) {
            uint64_t s = (uint64_t)acc->w[i] + (i < t->n ? t->w[i] : 0) + c;
            acc->w[i] = (uint32_t)s;
            c = s >> 32;
        }
        acc->w[L] = (uint32_t)c;
        acc->n = L + 1;
        a1164_norm(acc);
    } else {
        int cmp = a1164_cmpmag(acc, t);
        uint64_t borrow = 0;
        if (cmp == 0) { acc->n = 0; acc->neg = 0; return; }
        if (cmp > 0) {
            for (i = 0; i < acc->n; i++) {
                uint64_t s = (uint64_t)(i < t->n ? t->w[i] : 0) + borrow;
                if ((uint64_t)acc->w[i] >= s) { acc->w[i] = (uint32_t)(acc->w[i] - s); borrow = 0; }
                else { acc->w[i] = (uint32_t)((uint64_t)acc->w[i] + 0x100000000ULL - s); borrow = 1; }
            }
            a1164_norm(acc);
        } else {
            a1164_reserve(acc, t->n);
            for (i = acc->n; i < t->n; i++) acc->w[i] = 0;
            for (i = 0; i < t->n; i++) {
                uint64_t s = (uint64_t)acc->w[i] + borrow;
                if ((uint64_t)t->w[i] >= s) { acc->w[i] = (uint32_t)(t->w[i] - s); borrow = 0; }
                else { acc->w[i] = (uint32_t)((uint64_t)t->w[i] + 0x100000000ULL - s); borrow = 1; }
            }
            acc->n = t->n;
            acc->neg = t->neg;
            a1164_norm(acc);
        }
    }
}

/* primes */
static int *P_pr = NULL, *P_idx = NULL;
static int P_np = 0;
static uint64_t P_lim = 0;

static void a1164_primes(uint64_t lim) {
    char *comp;
    uint64_t i, j;
    if (lim <= P_lim) return;
    comp = (char *)calloc(lim + 1, 1);
    free(P_pr);
    free(P_idx);
    P_pr = (int *)malloc((lim + 1) * sizeof(int));
    P_idx = (int *)malloc((lim + 1) * sizeof(int));
    P_np = 0;
    for (i = 0; i <= lim; i++) P_idx[i] = -1;
    for (i = 2; i <= lim; i++) {
        if (!comp[i]) {
            P_idx[i] = P_np;
            P_pr[P_np++] = (int)i;
            for (j = i * i; j <= lim; j += i) comp[j] = 1;
        }
    }
    free(comp);
    P_lim = lim;
}

static void a1164_addfac(int *v, uint64_t x) {
    int i;
    for (i = 0; i < P_np && (uint64_t)P_pr[i] * (uint64_t)P_pr[i] <= x; i++) {
        uint64_t p = (uint64_t)P_pr[i];
        while (x % p == 0) { v[i]++; x /= p; }
    }
    if (x > 1) v[P_idx[x]]++;
}

/* memoized h(k) = (-1)^neg * U / prod p^E */
static a1164_Z *H_U = NULL;
static int **H_E = NULL;
static int *H_el = NULL;
static uint64_t H_cnt = 0, H_cap = 0;

static void a1164_addvec(int *d, uint64_t k) {
    int i;
    for (i = 0; i < H_el[k]; i++) d[i] += H_E[k][i];
}

static void a1164_termden(uint64_t k, uint64_t j, int *d) {
    memset(d, 0, (size_t)P_np * sizeof(int));
    if (j == 0) {
        a1164_addvec(d, k - 1);
        a1164_addfac(d, k);
        a1164_addfac(d, k + 2);
    } else {
        a1164_addvec(d, j);
        a1164_addvec(d, k - j);
        a1164_addfac(d, j + 1);
        a1164_addfac(d, k - j + 1);
        if (2 * j == k) a1164_addfac(d, 2);
    }
}

static void a1164_compute(uint64_t k) {
    int np = P_np, i;
    int *E = (int *)calloc((size_t)np + 1, sizeof(int));
    int *d = (int *)malloc(((size_t)np + 1) * sizeof(int));
    uint64_t jmax = k / 2, j;
    a1164_Z acc, t;
    a1164_init(&acc);
    a1164_init(&t);

    for (j = 0; j <= jmax; j++) {
        a1164_termden(k, j, d);
        for (i = 0; i < np; i++) if (d[i] > E[i]) E[i] = d[i];
    }
    for (j = 0; j <= jmax; j++) {
        uint64_t m = 1;
        a1164_termden(k, j, d);
        if (j == 0) {
            a1164_copy(&t, &H_U[k - 1]);
        } else {
            a1164_mul(&t, &H_U[j], &H_U[k - j]);
            if (t.n) t.neg = !t.neg;
        }
        a1164_muls(&t, (uint32_t)(k + 1));
        for (i = 0; i < np; i++) {
            int diff = E[i] - d[i];
            uint64_t p = (uint64_t)P_pr[i];
            while (diff-- > 0) {
                if (m * p > 0xFFFFFFFFULL) { a1164_muls(&t, (uint32_t)m); m = 1; }
                m *= p;
            }
        }
        if (m > 1) a1164_muls(&t, (uint32_t)m);
        a1164_addto(&acc, &t);
    }
    for (i = 0; i < np; i++) {
        uint32_t p = (uint32_t)P_pr[i];
        while (E[i] > 0 && acc.n && a1164_mods(&acc, p) == 0) {
            a1164_divs(&acc, p);
            E[i]--;
        }
    }
    free(d);
    free(t.w);
    H_U[k] = acc;
    H_E[k] = E;
    H_el[k] = np;
}

seq_t a(idx_t n) {
    uint64_t N, k, M;
    int i;
    seq_t r = 1;
    if (n == 0) return 1;
    N = 2 * (uint64_t)n;
    a1164_primes(N + 2);
    if (H_cap < N + 1) {
        uint64_t q;
        H_U = (a1164_Z *)realloc(H_U, (N + 1) * sizeof(a1164_Z));
        H_E = (int **)realloc(H_E, (N + 1) * sizeof(int *));
        H_el = (int *)realloc(H_el, (N + 1) * sizeof(int));
        for (q = H_cap; q <= N; q++) {
            a1164_init(&H_U[q]);
            H_E[q] = NULL;
            H_el[q] = 0;
        }
        H_cap = N + 1;
    }
    if (H_cnt == 0) {
        a1164_reserve(&H_U[0], 1);
        H_U[0].w[0] = 1;
        H_U[0].n = 1;
        H_U[0].neg = 0;
        H_E[0] = NULL;
        H_el[0] = 0;
        H_cnt = 1;
    }
    for (k = H_cnt; k <= N; k++) a1164_compute(k);
    if (H_cnt < N + 1) H_cnt = N + 1;

    M = N - 1; /* (2n-1)!! */
    for (i = 0; i < H_el[N]; i++) {
        uint64_t p = (uint64_t)P_pr[i];
        long long e = H_E[N][i];
        if (e <= 0) continue;
        if (p != 2) {
            uint64_t q = p;
            long long v = 0;
            while (q <= M) {
                uint64_t c = M / q;
                v += (long long)((c + 1) / 2);
                if (q > M / p) break;
                q *= p;
            }
            e -= v;
        }
        while (e-- > 0) r *= (seq_t)p;
    }
    return r;
}
