/*
 * A007320  Number of steps needed for juggler sequence (A094683) started at n
 *          to reach 1.
 * https://oeis.org/A007320
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = number of iterations of x -> floor(sqrt(x)) for even x
 *            and x -> floor(x^(3/2)) = floor(sqrt(x^3)) for odd x, starting
 *            at x = n, until x = 1; simulated with exact internal bignums
 *            since trajectories exceed 64 bits
 * Range:     u8 n <= 10000, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   jb_clz32
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include "../utils/jb_clz32.h"

typedef struct { uint32_t *d; size_t n; } jbig;

static void jb_trim(jbig *a) {
    while (a->n && a->d[a->n - 1] == 0) a->n--;
}

static jbig jb_alloc(size_t n) {
    jbig r;
    r.n = n;
    r.d = (uint32_t *)calloc(n ? n : 1, sizeof(uint32_t));
    return r;
}

static int jb_cmp(jbig a, jbig b) {
    if (a.n != b.n) return a.n < b.n ? -1 : 1;
    for (size_t i = a.n; i-- > 0;) {
        if (a.d[i] != b.d[i]) return a.d[i] < b.d[i] ? -1 : 1;
    }
    return 0;
}

static jbig jb_mul(jbig a, jbig b) {
    jbig r = jb_alloc(a.n + b.n);
    for (size_t i = 0; i < a.n; i++) {
        uint64_t c = 0;
        for (size_t j = 0; j < b.n; j++) {
            uint64_t t = (uint64_t)a.d[i] * b.d[j] + r.d[i + j] + c;
            r.d[i + j] = (uint32_t)t;
            c = t >> 32;
        }
        r.d[i + b.n] = (uint32_t)c;
    }
    jb_trim(&r);
    return r;
}

static jbig jb_add_shr1(jbig a, jbig b) {
    size_t m = a.n > b.n ? a.n : b.n;
    jbig r = jb_alloc(m + 1);
    uint64_t c = 0;
    for (size_t i = 0; i < m; i++) {
        uint64_t t = c;
        if (i < a.n) t += a.d[i];
        if (i < b.n) t += b.d[i];
        r.d[i] = (uint32_t)t;
        c = t >> 32;
    }
    r.d[m] = (uint32_t)c;
    for (size_t i = 0; i < m; i++)
        r.d[i] = (r.d[i] >> 1) | (r.d[i + 1] << 31);
    r.d[m] >>= 1;
    jb_trim(&r);
    return r;
}

/* quotient floor(u / v); v nonzero, both trimmed */
static jbig jb_div(jbig u, jbig v) {
    if (u.n < v.n) { jbig z = jb_alloc(0); return z; }
    size_t m = u.n, nn = v.n;
    jbig q = jb_alloc(m - nn + 1);
    if (nn == 1) {
        uint64_t r = 0;
        for (size_t i = m; i-- > 0;) {
            uint64_t cur = (r << 32) | u.d[i];
            q.d[i] = (uint32_t)(cur / v.d[0]);
            r = cur % v.d[0];
        }
        jb_trim(&q);
        return q;
    }
    const uint64_t B = (uint64_t)1 << 32;
    int s = jb_clz32(v.d[nn - 1]);
    uint32_t *vn = (uint32_t *)malloc(nn * sizeof(uint32_t));
    uint32_t *un = (uint32_t *)malloc((m + 1) * sizeof(uint32_t));
    for (size_t i = nn - 1; i > 0; i--)
        vn[i] = (v.d[i] << s) | (s ? v.d[i - 1] >> (32 - s) : 0);
    vn[0] = v.d[0] << s;
    un[m] = s ? u.d[m - 1] >> (32 - s) : 0;
    for (size_t i = m - 1; i > 0; i--)
        un[i] = (u.d[i] << s) | (s ? u.d[i - 1] >> (32 - s) : 0);
    un[0] = u.d[0] << s;

    for (long long jj = (long long)(m - nn); jj >= 0; jj--) {
        size_t j = (size_t)jj;
        uint64_t num = ((uint64_t)un[j + nn] << 32) | un[j + nn - 1];
        uint64_t qhat = num / vn[nn - 1];
        uint64_t rhat = num % vn[nn - 1];
        while (qhat >= B || qhat * vn[nn - 2] > ((rhat << 32) | un[j + nn - 2])) {
            qhat--;
            rhat += vn[nn - 1];
            if (rhat >= B) break;
        }
        int64_t k = 0, t;
        for (size_t i = 0; i < nn; i++) {
            uint64_t p = qhat * vn[i];
            t = (int64_t)un[i + j] - k - (int64_t)(p & 0xFFFFFFFFu);
            un[i + j] = (uint32_t)t;
            k = (int64_t)(p >> 32) - (t >> 32);
        }
        t = (int64_t)un[j + nn] - k;
        un[j + nn] = (uint32_t)t;
        q.d[j] = (uint32_t)qhat;
        if (t < 0) {
            q.d[j]--;
            uint64_t c = 0;
            for (size_t i = 0; i < nn; i++) {
                uint64_t tt = (uint64_t)un[i + j] + vn[i] + c;
                un[i + j] = (uint32_t)tt;
                c = tt >> 32;
            }
            un[j + nn] += (uint32_t)c;
        }
    }
    free(vn);
    free(un);
    jb_trim(&q);
    return q;
}

static jbig jb_isqrt(jbig N) {
    if (N.n == 0) return jb_alloc(0);
    size_t bits = 32 * (N.n - 1) + (size_t)(32 - jb_clz32(N.d[N.n - 1]));
    size_t e = (bits + 1) / 2;
    jbig x = jb_alloc(e / 32 + 1);
    x.d[e / 32] = (uint32_t)1 << (e % 32);
    for (;;) {
        jbig q = jb_div(N, x);
        jbig y = jb_add_shr1(x, q);
        free(q.d);
        if (jb_cmp(y, x) >= 0) { free(y.d); break; }
        free(x.d);
        x = y;
    }
    return x;
}

seq_t a(idx_t n) {
    jbig x = jb_alloc(2);
    x.d[0] = (uint32_t)n;
    x.d[1] = (uint32_t)((uint64_t)n >> 32);
    jb_trim(&x);
    seq_t steps = 0;
    while (!(x.n == 1 && x.d[0] == 1)) {
        jbig nx;
        if ((x.d[0] & 1) == 0) {
            nx = jb_isqrt(x);
        } else {
            jbig sq = jb_mul(x, x);
            jbig cu = jb_mul(sq, x);
            free(sq.d);
            nx = jb_isqrt(cu);
            free(cu.d);
        }
        free(x.d);
        x = nx;
        steps++;
    }
    free(x.d);
    return steps;
}
