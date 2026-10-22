/*
 * A005148  Sequence of coefficients arising in connection with a rapidly
 *          converging series for Pi.
 * https://oeis.org/A005148
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy, nice
 * Formula:   a(n) = (1/24)*binomial(2n,n)*(16^n - binomial(2n,n)^2) -
 *            Sum_{i=0..n-1} binomial(2n-2i,n-i)^3 * a(i), a(0) = 0 (Shanks
 *            and Beukers, from A076657); (1/24)*b*(16^n-b^2) is computed as
 *            (b/2)*((4^n-b)/2)*((4^n+b)/2)/3
 * Range:     u8 n <= 2, u16 n <= 3, u32 n <= 6, u64 n <= 11, u128 n <= 22,
 *            big any n
 * Helpers:   alloc_zero, grow_zero, seq_is_fixed_width
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stddef.h>
#include <stdbool.h>
#include "../utils/alloc_zero.h"
#include "../utils/grow_zero.h"
#include "../utils/seq_is_fixed_width.h"

static seq_t *av = NULL;   /* a(k) */
static seq_t *cb = NULL;   /* binomial(2k,k) */
static seq_t *c3 = NULL;   /* binomial(2k,k)^3 */
static idx_t done = 0;     /* number of terms computed */
static idx_t cap = 0;
static seq_t p4 = 1;       /* 4^(done-1) */
static seq_t inv3 = 0;
static bool inv3_ready = false;

static seq_t mul(seq_t x, seq_t y) {
    return (seq_t)(x * (y + 0u));
}

static void ensure_cap(idx_t need) {
    idx_t nc;
    if (need <= cap) return;
    nc = cap ? cap : 64;
    while (nc < need) nc *= 2;
    if (av == NULL) {
        av = alloc_zero(nc);
        cb = alloc_zero(nc);
        c3 = alloc_zero(nc);
    } else {
        av = grow_zero(av, cap, nc);
        cb = grow_zero(cb, cap, nc);
        c3 = grow_zero(c3, cap, nc);
    }
    cap = nc;
}

seq_t a(idx_t n) {
    idx_t k, i;
    if (!inv3_ready) {
        if (seq_is_fixed_width()) {
            /* inverse of 3 modulo 2^w by Newton iteration */
            seq_t x = 3;
            int t;
            for (t = 0; t < 8; t++)
                x = mul(x, (seq_t)(2 - mul((seq_t)3, x)));
            inv3 = x;
        }
        inv3_ready = true;
    }
    ensure_cap(n + 1);
    for (k = done; k <= n; k++) {
        if (k == 0) {
            cb[0] = 1;
            c3[0] = 1;
            av[0] = 0;
            p4 = 1;
        } else {
            seq_t h, f1, f2, prod, e, s;
            cb[k] = (seq_t)(cb[k - 1] * (4 * k - 2) / k);
            c3[k] = mul(mul(cb[k], cb[k]), cb[k]);
            p4 = mul(p4, (seq_t)4);
            h = cb[k] / 2;
            f1 = (seq_t)(p4 - cb[k]) / 2;
            f2 = (seq_t)(p4 + cb[k]) / 2;
            prod = mul(mul(h, f1), f2);
            if (seq_is_fixed_width())
                e = mul(prod, inv3);
            else
                e = prod / 3;
            s = 0;
            for (i = 1; i < k; i++)
                s = (seq_t)(s + mul(c3[k - i], av[i]));
            av[k] = (seq_t)(e - s);
        }
        done = k + 1;
    }
    return av[n];
}
