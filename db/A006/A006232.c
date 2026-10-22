/*
 * A006232  Numerators of Cauchy numbers of first type.
 * https://oeis.org/A006232
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign, frac, nice
 * Formula:   a(n) = numerator(Sum_{k=0..n} Stirling1(n,k)/(k+1)), using
 *            signed Stirling1 = A048994
 * Range:     i8 n <= 5, i16 n <= 7, i32 n <= 11, i64 n <= 19, i128 n <= 33,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>
#include <string.h>

typedef struct {
    uint32_t *d;
    size_t n;
} a6232_bn;

static void a6232_trim(a6232_bn *a) {
    while (a->n && a->d[a->n - 1] == 0) a->n--;
}

static void a6232_mul_small(a6232_bn *a, uint32_t m) {
    uint64_t c = 0;
    size_t i;
    for (i = 0; i < a->n; i++) {
        uint64_t t = (uint64_t)a->d[i] * m + c;
        a->d[i] = (uint32_t)t;
        c = t >> 32;
    }
    if (c) a->d[a->n++] = (uint32_t)c;
    a6232_trim(a);
}

static void a6232_add(a6232_bn *a, const a6232_bn *b) {
    size_t mx = a->n > b->n ? a->n : b->n;
    uint64_t c = 0;
    size_t i;
    for (i = 0; i < mx; i++) {
        uint64_t t = c + (i < a->n ? a->d[i] : 0) + (i < b->n ? b->d[i] : 0);
        a->d[i] = (uint32_t)t;
        c = t >> 32;
    }
    a->n = mx;
    if (c) a->d[a->n++] = (uint32_t)c;
}

static int a6232_cmp(const a6232_bn *a, const a6232_bn *b) {
    size_t i;
    if (a->n != b->n) return a->n < b->n ? -1 : 1;
    for (i = a->n; i > 0; i--) {
        if (a->d[i - 1] != b->d[i - 1]) return a->d[i - 1] < b->d[i - 1] ? -1 : 1;
    }
    return 0;
}

/* a -= b, requires a >= b */
static void a6232_sub(a6232_bn *a, const a6232_bn *b) {
    int64_t br = 0;
    size_t i;
    for (i = 0; i < a->n; i++) {
        int64_t t = (int64_t)a->d[i] - (int64_t)(i < b->n ? b->d[i] : 0) - br;
        if (t < 0) {
            t += (int64_t)1 << 32;
            br = 1;
        } else {
            br = 0;
        }
        a->d[i] = (uint32_t)t;
    }
    a6232_trim(a);
}

/* acc += a * b; limbs of acc beyond acc->n are zero */
static void a6232_mul_add(a6232_bn *acc, const a6232_bn *a, const a6232_bn *b) {
    size_t i, j, k, top;
    if (a->n == 0 || b->n == 0) return;
    for (i = 0; i < a->n; i++) {
        uint64_t c = 0;
        uint64_t ai = a->d[i];
        for (j = 0; j < b->n; j++) {
            uint64_t t = ai * b->d[j] + acc->d[i + j] + c;
            acc->d[i + j] = (uint32_t)t;
            c = t >> 32;
        }
        k = i + b->n;
        while (c) {
            uint64_t t = (uint64_t)acc->d[k] + c;
            acc->d[k] = (uint32_t)t;
            c = t >> 32;
            k++;
        }
    }
    top = a->n + b->n + 1;
    if (acc->n < top) acc->n = top;
    a6232_trim(acc);
}

static uint32_t a6232_div_small(a6232_bn *a, uint32_t p) {
    uint64_t r = 0;
    size_t i;
    for (i = a->n; i > 0; i--) {
        r = (r << 32) | a->d[i - 1];
        a->d[i - 1] = (uint32_t)(r / p);
        r %= p;
    }
    a6232_trim(a);
    return (uint32_t)r;
}

static uint32_t a6232_mod_small(const a6232_bn *a, uint32_t p) {
    uint64_t r = 0;
    size_t i;
    for (i = a->n; i > 0; i--) {
        r = ((r << 32) | a->d[i - 1]) % p;
    }
    return (uint32_t)r;
}

static a6232_bn a6232_new(size_t w) {
    a6232_bn b;
    b.d = (uint32_t *)calloc(w, sizeof(uint32_t));
    b.n = 0;
    return b;
}

seq_t a(idx_t n) {
    size_t N = (size_t)n;
    size_t bl = 0, x = N, W, k, m, p;
    a6232_bn *c, L, Q, P, M;
    char *comp;
    int neg;
    a6232_bn *R;
    seq_t r = 0;

    while (x) {
        bl++;
        x >>= 1;
    }
    W = (N * (bl + 1) + 2 * (N + 1) + 64) / 32 + 8;

    /* unsigned Stirling numbers of the first kind, row n */
    c = (a6232_bn *)malloc((N + 1) * sizeof(a6232_bn));
    for (k = 0; k <= N; k++) c[k] = a6232_new(W);
    c[0].d[0] = 1;
    c[0].n = 1;
    for (m = 0; m < N; m++) {
        for (k = m + 1; k >= 1; k--) {
            a6232_mul_small(&c[k], (uint32_t)m);
            a6232_add(&c[k], &c[k - 1]);
        }
        a6232_mul_small(&c[0], (uint32_t)m);
    }

    /* primes up to n+1 and L = lcm(1..n+1) */
    comp = (char *)calloc(N + 2, 1);
    L = a6232_new(W);
    L.d[0] = 1;
    L.n = 1;
    for (p = 2; p <= N + 1; p++) {
        if (comp[p]) continue;
        for (k = p * p; k <= N + 1; k += p) comp[k] = 1;
        {
            size_t q = p;
            while (q <= N + 1) {
                a6232_mul_small(&L, (uint32_t)p);
                q *= p;
            }
        }
    }

    /* N = Sum_k (-1)^(n-k) c(n,k) * L/(k+1) */
    Q = a6232_new(W);
    P = a6232_new(W);
    M = a6232_new(W);
    for (k = 0; k <= N; k++) {
        if (c[k].n == 0) continue;
        memcpy(Q.d, L.d, W * sizeof(uint32_t));
        Q.n = L.n;
        a6232_div_small(&Q, (uint32_t)(k + 1));
        if (((N - k) & 1) == 0)
            a6232_mul_add(&P, &c[k], &Q);
        else
            a6232_mul_add(&M, &c[k], &Q);
    }
    if (a6232_cmp(&P, &M) >= 0) {
        a6232_sub(&P, &M);
        R = &P;
        neg = 0;
    } else {
        a6232_sub(&M, &P);
        R = &M;
        neg = 1;
    }

    /* reduce by common factors with L */
    if (R->n) {
        for (p = 2; p <= N + 1; p++) {
            size_t q, e = 0;
            if (comp[p]) continue;
            q = p;
            while (q <= N + 1) {
                e++;
                q *= p;
            }
            while (e > 0 && a6232_mod_small(R, (uint32_t)p) == 0) {
                a6232_div_small(R, (uint32_t)p);
                e--;
            }
        }
    }

    /* convert to seq_t, 4 bits at a time */
    {
        size_t i;
        int sh;
        for (i = R->n; i > 0; i--) {
            uint32_t limb = R->d[i - 1];
            for (sh = 28; sh >= 0; sh -= 4) {
                int nib = (int)((limb >> sh) & 15u);
                if (neg)
                    r = r * 16 - nib;
                else
                    r = r * 16 + nib;
            }
        }
    }

    for (k = 0; k <= N; k++) free(c[k].d);
    free(c);
    free(comp);
    free(L.d);
    free(Q.d);
    free(P.d);
    free(M.d);
    return r;
}
