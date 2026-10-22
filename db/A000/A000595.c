/*
 * A000595  Number of binary relations on n unlabeled points.
 * https://oeis.org/A000595
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, nice
 * Formula:   a(n) = Sum_{partitions 1*s_1+2*s_2+...=n} 2^(Sum_{i,j>=1}
 *            gcd(i,j)*s_i*s_j) / (1^s_1*s_1!*2^s_2*s_2!*...)
 * Range:     u8 n <= 3, u16 n <= 4, u32 n <= 6, u64 n <= 9, u128 n <= 12,
 *            big any n
 * Helpers:   r595_gcd, r595_inv, r595_mul
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>
#include <stdlib.h>
#include "../utils/r595_gcd.h"
#include "../utils/r595_inv.h"
#include "../utils/r595_mul.h"

static int r595_wcache = 0; /* 0 unknown, -1 arbitrary precision, else bit width */

static int r595_width(void) {
    if (r595_wcache == 0) {
        seq_t x = 1;
        int k = 0, w = -1;
        while (k < 1100) {
            x = (seq_t)(x << 1);
            k++;
            if (x == 0) { w = k; break; }
        }
        r595_wcache = w;
    }
    return r595_wcache;
}

typedef struct {
    idx_t *p;
    idx_t *s;
    int depth;
    int w;
    seq_t *sum;
} r595_ctx;

/* arbitrary precision: C = n! / prod(j^s_j * s_j!) so far; leaf adds C * 2^e */
static void r595_big(r595_ctx *c, idx_t rem, idx_t maxp, uint64_t e, seq_t C) {
    if (rem == 0) {
        *c->sum += C << e;
        return;
    }
    idx_t top = maxp < rem ? maxp : rem;
    for (idx_t j = top; j >= 1; j--) {
        uint64_t g = 0;
        for (int d = 0; d < c->depth; d++)
            g += r595_gcd(c->p[d], j) * c->s[d];
        seq_t Cj = C;
        for (idx_t s = 1; s * j <= rem; s++) {
            Cj /= j;
            Cj /= s;
            uint64_t ej = e + s * s * j + 2 * s * g;
            c->p[c->depth] = j;
            c->s[c->depth] = s;
            c->depth++;
            r595_big(c, rem - s * j, j - 1, ej, Cj);
            c->depth--;
        }
    }
}

/* fixed width: z = 2^t * m (m odd); leaf adds 2^(e-t) * m^-1 mod 2^w */
static void r595_fix(r595_ctx *c, idx_t rem, idx_t maxp, uint64_t e,
                     uint64_t t, seq_t m) {
    if (rem == 0) {
        uint64_t k = e - t;
        if (k < (uint64_t)c->w) {
            seq_t term = (seq_t)((seq_t)1 << k);
            *c->sum += r595_mul(term, r595_inv(m));
        }
        return;
    }
    idx_t top = maxp < rem ? maxp : rem;
    for (idx_t j = top; j >= 1; j--) {
        uint64_t g = 0;
        for (int d = 0; d < c->depth; d++)
            g += r595_gcd(c->p[d], j) * c->s[d];
        uint64_t jo = j, jt = 0;
        while ((jo & 1) == 0) { jo >>= 1; jt++; }
        seq_t mj = m;
        uint64_t tj = t;
        for (idx_t s = 1; s * j <= rem; s++) {
            uint64_t so = s, st = 0;
            while ((so & 1) == 0) { so >>= 1; st++; }
            mj = r595_mul(mj, (seq_t)jo);
            mj = r595_mul(mj, (seq_t)so);
            tj += jt + st;
            uint64_t ej = e + s * s * j + 2 * s * g;
            c->p[c->depth] = j;
            c->s[c->depth] = s;
            c->depth++;
            r595_fix(c, rem - s * j, j - 1, ej, tj, mj);
            c->depth--;
        }
    }
}

seq_t a(idx_t n) {
    int w = r595_width();
    seq_t sum = 0;
    r595_ctx c;
    c.p = (idx_t *)malloc((n + 1) * sizeof(idx_t));
    c.s = (idx_t *)malloc((n + 1) * sizeof(idx_t));
    c.depth = 0;
    c.w = w;
    c.sum = &sum;
    seq_t result;
    if (w < 0) {
        seq_t f = 1;
        for (idx_t i = 2; i <= n; i++) f *= i;
        r595_big(&c, n, n, 0, f);
        result = sum / f;
    } else {
        r595_fix(&c, n, n, 0, 0, (seq_t)1);
        result = sum;
    }
    free(c.p);
    free(c.s);
    return result;
}
