/*
 * A005447  Numerators of the expansion of -W_{-1}(-e^(-1 - x^2/2)) where x >
 *          0 and W_{-1} is the Lambert W function.
 * https://oeis.org/A005447
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign, frac
 * Formula:   a(n) = numerator(c(n)), where log(A(x)) = A(x) - 1 - x^2/2 with
 *            A(x) = Sum c(n)*x^n. Differentiating gives A'(x)*(A(x)-1) =
 *            x*A(x), which leads to the recurrence c(0) = c(1) = 1, c(n) =
 *            c(n-1)/(n+1) - Sum_{2<=j<k, j+k=n+1} c(j)*c(k) - [n
 *            odd]*c((n+1)/2)^2/2. The code uses exact rationals: the num...
 * Range:     i8 n <= 6, i16 n <= 10, i32 n <= 16, i64 n <= 22, i128 n <= 36,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>
#include <string.h>

typedef struct { uint32_t *d; size_t n; int neg; } bnum;

static void bn_trim(bnum *a) {
    while (a->n && a->d[a->n - 1] == 0) a->n--;
    if (!a->n) a->neg = 0;
}

static bnum bn_small(uint32_t v) {
    bnum r;
    r.d = (uint32_t *)malloc(sizeof(uint32_t));
    r.d[0] = v; r.n = v ? 1 : 0; r.neg = 0;
    return r;
}

static bnum bn_copy(const bnum *a) {
    bnum r;
    r.d = (uint32_t *)malloc((a->n ? a->n : 1) * sizeof(uint32_t));
    if (a->n) memcpy(r.d, a->d, a->n * sizeof(uint32_t));
    r.n = a->n; r.neg = a->neg;
    return r;
}

static bnum bn_mul(const bnum *a, const bnum *b) {
    bnum r;
    size_t rn = a->n + b->n;
    r.d = (uint32_t *)calloc(rn ? rn : 1, sizeof(uint32_t));
    r.n = rn;
    for (size_t i = 0; i < a->n; i++) {
        uint64_t c = 0;
        for (size_t j = 0; j < b->n; j++) {
            uint64_t t = (uint64_t)a->d[i] * b->d[j] + r.d[i + j] + c;
            r.d[i + j] = (uint32_t)t;
            c = t >> 32;
        }
        r.d[i + b->n] = (uint32_t)c;
    }
    r.neg = a->neg ^ b->neg;
    bn_trim(&r);
    return r;
}

static void bn_mul_small(bnum *a, uint32_t k) {
    if (!a->n) return;
    a->d = (uint32_t *)realloc(a->d, (a->n + 1) * sizeof(uint32_t));
    uint64_t c = 0;
    for (size_t i = 0; i < a->n; i++) {
        uint64_t t = (uint64_t)a->d[i] * k + c;
        a->d[i] = (uint32_t)t;
        c = t >> 32;
    }
    a->d[a->n] = (uint32_t)c;
    a->n++;
    bn_trim(a);
}

static uint32_t bn_mod_small(const bnum *a, uint32_t p) {
    uint64_t r = 0;
    for (size_t i = a->n; i-- > 0;) r = ((r << 32) | a->d[i]) % p;
    return (uint32_t)r;
}

static void bn_div_small(bnum *a, uint32_t p) {
    uint64_t r = 0;
    for (size_t i = a->n; i-- > 0;) {
        uint64_t cur = (r << 32) | a->d[i];
        a->d[i] = (uint32_t)(cur / p);
        r = cur % p;
    }
    bn_trim(a);
}

static int mag_cmp(const bnum *a, const bnum *b) {
    if (a->n != b->n) return a->n > b->n ? 1 : -1;
    for (size_t i = a->n; i-- > 0;)
        if (a->d[i] != b->d[i]) return a->d[i] > b->d[i] ? 1 : -1;
    return 0;
}

/* acc += (tneg ? -|t| : |t|) */
static void bn_add_to(bnum *acc, const bnum *t, int tneg) {
    if (!t->n) return;
    if (!acc->n) {
        free(acc->d);
        *acc = bn_copy(t);
        acc->neg = tneg;
        return;
    }
    size_t m = (acc->n > t->n ? acc->n : t->n) + 1;
    uint32_t *r = (uint32_t *)calloc(m, sizeof(uint32_t));
    int neg;
    if (acc->neg == tneg) {
        uint64_t c = 0;
        for (size_t i = 0; i + 1 < m; i++) {
            uint64_t s = (uint64_t)(i < acc->n ? acc->d[i] : 0) + (i < t->n ? t->d[i] : 0) + c;
            r[i] = (uint32_t)s;
            c = s >> 32;
        }
        r[m - 1] = (uint32_t)c;
        neg = acc->neg;
    } else {
        int cmp = mag_cmp(acc, t);
        if (cmp == 0) {
            neg = 0;
        } else {
            const bnum *big = cmp > 0 ? acc : t;
            const bnum *sm = cmp > 0 ? t : acc;
            int64_t br = 0;
            for (size_t i = 0; i < big->n; i++) {
                int64_t s = (int64_t)big->d[i] - (int64_t)(i < sm->n ? sm->d[i] : 0) - br;
                if (s < 0) { s += (int64_t)4294967296LL; br = 1; } else br = 0;
                r[i] = (uint32_t)s;
            }
            neg = cmp > 0 ? acc->neg : tneg;
        }
    }
    free(acc->d);
    acc->d = r; acc->n = m; acc->neg = neg;
    bn_trim(acc);
}

static bnum *nums = NULL;
static int **exs = NULL;
static size_t *nes = NULL;
static idx_t have = 0;

static uint32_t *pr = NULL;
static size_t npr = 0, prcap = 0;
static uint32_t pr_checked = 1;

static void ensure_primes(uint32_t L) {
    for (uint32_t c = pr_checked + 1; c <= L; c++) {
        int isp = 1;
        for (size_t i = 0; i < npr && (uint64_t)pr[i] * pr[i] <= c; i++)
            if (c % pr[i] == 0) { isp = 0; break; }
        if (isp) {
            if (npr == prcap) {
                prcap = prcap ? 2 * prcap : 64;
                pr = (uint32_t *)realloc(pr, prcap * sizeof(uint32_t));
            }
            pr[npr++] = c;
        }
    }
    if (L > pr_checked) pr_checked = L;
}

static int getex(idx_t j, size_t i) {
    return i < nes[j] ? exs[j][i] : 0;
}

/* kind 0: c(m-1)/(m+1); kind 1: c(j)*c(m+1-j); kind 2: c(j)^2/2 */
static void term_ex(int kind, idx_t m, idx_t j, size_t P, int *out) {
    for (size_t i = 0; i < P; i++) {
        if (kind == 0) {
            int v = 0;
            uint64_t q = (uint64_t)m + 1;
            while (q % pr[i] == 0) { q /= pr[i]; v++; }
            out[i] = getex(m - 1, i) + v;
        } else if (kind == 1) {
            out[i] = getex(j, i) + getex(m + 1 - j, i);
        } else {
            out[i] = 2 * getex(j, i) + (pr[i] == 2 ? 1 : 0);
        }
    }
}

static bnum term_num(int kind, idx_t m, idx_t j) {
    if (kind == 0) return bn_copy(&nums[m - 1]);
    if (kind == 1) return bn_mul(&nums[j], &nums[m + 1 - j]);
    return bn_mul(&nums[j], &nums[j]);
}

static void apply_cof(bnum *x, const int *E, const int *te, size_t P) {
    uint64_t mult = 1;
    for (size_t i = 0; i < P; i++) {
        int c = E[i] - te[i];
        while (c-- > 0) {
            if (mult * pr[i] > 0xFFFFFFFFULL) { bn_mul_small(x, (uint32_t)mult); mult = 1; }
            mult *= pr[i];
        }
    }
    if (mult > 1) bn_mul_small(x, (uint32_t)mult);
}

static void step(idx_t m) {
    ensure_primes((uint32_t)(m + 1));
    size_t P = 0;
    while (P < npr && pr[P] <= m + 1) P++;
    int *E = (int *)calloc(P ? P : 1, sizeof(int));
    int *t = (int *)malloc((P ? P : 1) * sizeof(int));
    int pass;
    bnum N = bn_small(0);
    for (pass = 0; pass < 2; pass++) {
        idx_t j;
        for (int kind = 0; kind < 3; kind++) {
            idx_t jlo, jhi;
            if (kind == 0) { jlo = 0; jhi = 1; }
            else if (kind == 1) { jlo = 2; jhi = (m + 2) / 2; }
            else {
                if ((m + 1) % 2 != 0) continue;
                jlo = (m + 1) / 2; jhi = jlo + 1;
            }
            for (j = jlo; j < jhi; j++) {
                if (kind == 1 && !(j + j < m + 1)) break;
                term_ex(kind, m, j, P, t);
                if (pass == 0) {
                    for (size_t i = 0; i < P; i++) if (t[i] > E[i]) E[i] = t[i];
                } else {
                    bnum x = term_num(kind, m, j);
                    apply_cof(&x, E, t, P);
                    bn_add_to(&N, &x, kind == 0 ? x.neg : (x.neg ^ 1));
                    free(x.d);
                }
            }
        }
    }
    if (!N.n) {
        for (size_t i = 0; i < P; i++) E[i] = 0;
    } else {
        for (size_t i = 0; i < P; i++)
            while (E[i] > 0 && bn_mod_small(&N, pr[i]) == 0) {
                bn_div_small(&N, pr[i]);
                E[i]--;
            }
    }
    free(t);
    nums[m] = N;
    exs[m] = E;
    nes[m] = P;
}

seq_t a(idx_t n) {
    if (n >= have) {
        idx_t len = n + 1 < 2 ? 2 : n + 1;
        nums = (bnum *)realloc(nums, len * sizeof(bnum));
        exs = (int **)realloc(exs, len * sizeof(int *));
        nes = (size_t *)realloc(nes, len * sizeof(size_t));
        if (have == 0) {
            nums[0] = bn_small(1); exs[0] = NULL; nes[0] = 0;
            nums[1] = bn_small(1); exs[1] = NULL; nes[1] = 0;
            have = 2;
        }
        for (idx_t m = have; m <= n; m++) step(m);
        if (n + 1 > have) have = n + 1;
    }
    const bnum *x = &nums[n];
    seq_t r = 0;
    for (size_t i = x->n; i-- > 0;) {
        r = r * 65536 + (seq_t)(long long)(x->d[i] >> 16);
        r = r * 65536 + (seq_t)(long long)(x->d[i] & 0xFFFFu);
    }
    return x->neg ? -r : r;
}
