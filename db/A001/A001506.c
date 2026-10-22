/*
 * A001506  a(n) is the number of c-nets with n+1 vertices and 2n edges, n >=
 *          1.
 * https://oeis.org/A001506
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   for n >= 3, with k = n-1 and m = 2k+1 = 2n-1, a(n) = (
 *            3*Sum_{t=1..k-1} C(m,t)*C(2k-t-3,k-1-t)*C(k-2,t-1) -
 *            Sum_{t=2..k} C(m,t)*C(2k-t-2,k-t)*C(k-2,t-2) ) / m, a(1) = a(2)
 *            = 0. Derived from a(n) = -[x^k y^k] N(x,y)^(-m) / m, where N is
 *            the network g.f. of rooted nonseparable maps (Brown-Tutte par...
 * Range:     u8 n <= 6, u16 n <= 8, u32 n <= 13, u64 n <= 21, u128 n <= 38,
 *            big any n
 * Helpers:   a1506_inv, a1506_mul
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include "../utils/a1506_inv.h"
#include "../utils/a1506_mul.h"

typedef struct { seq_t v; int e; } a1506_t;

static int a1506_fixed_flag = -1;

static int a1506_isfixed(void)
{
    if (a1506_fixed_flag < 0) {
        seq_t p = 1;
        int i;
        for (i = 0; i < 256; i++) p = p + p;
        a1506_fixed_flag = (p == 0) ? 1 : 0;
    }
    return a1506_fixed_flag;
}

static void a1506_mulc(a1506_t *x, uint64_t c, int fixed)
{
    if (fixed) {
        while ((c & 1) == 0) { c >>= 1; x->e++; }
        x->v = a1506_mul(x->v, (seq_t)c);
    } else {
        x->v = x->v * (seq_t)c;
    }
}

static void a1506_divc(a1506_t *x, uint64_t c, int fixed)
{
    if (fixed) {
        while ((c & 1) == 0) { c >>= 1; x->e--; }
        x->v = a1506_mul(x->v, a1506_inv((seq_t)c));
    } else {
        x->v = x->v / (seq_t)c;
    }
}

static seq_t a1506_val(const a1506_t *x, int fixed)
{
    seq_t p = 1;
    int i;
    if (!fixed) return x->v;
    if (x->e >= 200) return 0;
    for (i = 0; i < x->e; i++) p = p + p;
    return a1506_mul(x->v, p);
}

seq_t a(idx_t n)
{
    int fixed;
    uint64_t k, m, i, t;
    a1506_t c, t1, t2;
    seq_t S1 = 0, S2 = 0, num;

    if (n < 3) return 0;
    fixed = a1506_isfixed();
    k = (uint64_t)n - 1;
    m = 2 * k + 1;

    /* c = C(2k-4, k-2) */
    c.v = 1; c.e = 0;
    for (i = 1; i + 2 <= k; i++) {
        a1506_mulc(&c, k - 2 + i, fixed);
        a1506_divc(&c, i, fixed);
    }

    /* term1 at t=2: C(m,2)*C(2k-4,k-2)*C(k-2,0) */
    t1.v = c.v; t1.e = c.e;
    a1506_mulc(&t1, m, fixed);
    a1506_mulc(&t1, m - 1, fixed);
    a1506_divc(&t1, 2, fixed);

    /* term2 at t=1: C(m,1)*C(2k-4,k-2)*C(k-2,0) */
    t2.v = c.v; t2.e = c.e;
    a1506_mulc(&t2, m, fixed);

    for (t = 2; t <= k; t++) {
        S1 += a1506_val(&t1, fixed);
        if (t < k) {
            a1506_mulc(&t1, m - t, fixed);
            a1506_mulc(&t1, k - t, fixed);
            a1506_mulc(&t1, k - t, fixed);
            a1506_divc(&t1, t + 1, fixed);
            a1506_divc(&t1, 2 * k - t - 2, fixed);
            a1506_divc(&t1, t - 1, fixed);
        }
    }

    for (t = 1; t + 1 <= k; t++) {
        S2 += a1506_val(&t2, fixed);
        if (t + 2 <= k) {
            a1506_mulc(&t2, m - t, fixed);
            a1506_mulc(&t2, k - 1 - t, fixed);
            a1506_mulc(&t2, k - 1 - t, fixed);
            a1506_divc(&t2, t + 1, fixed);
            a1506_divc(&t2, 2 * k - t - 3, fixed);
            a1506_divc(&t2, t, fixed);
        }
    }

    num = (seq_t)(S2 * 3 - S1);
    if (fixed) return a1506_mul(num, a1506_inv((seq_t)m));
    return num / (seq_t)m;
}
