/*
 * A005951  Theta series of P_{9a} packing.
 * https://oeis.org/A005951
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   theta series = E^9 + 18*O^4*E^5 + O^9, where E(q) = Sum_{j even}
 *            q^(j^2) = theta_3(q^4) and O(q) = Sum_{j odd} q^(j^2) =
 *            theta_2(q^4) (sums over all integers j). P_{9a} = {x in Z^9 : x
 *            mod 2 in C} union (1+2Z)^9, where C is the zero word together
 *            with 18 words of weight 4; a(n) = coefficient of q^n
 * Range:     u8 n <= 3, u16 n <= 19, u32 n <= 40, u64 n <= 40, u128 n <= 40,
 *            big any n
 * Helpers:   alloc_zero, release
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include "../utils/alloc_zero.h"
#include "../utils/release.h"

/* dst = src * (E or O), truncated at degree n */
static void mul_sparse(seq_t *dst, const seq_t *src, idx_t n, int odd)
{
    idx_t i, j;
    for (i = 0; i <= n; i++) {
        seq_t s = 0;
        if (!odd) s += src[i];
        for (j = odd ? 1 : 2; j * j <= i; j += 2)
            s += 2 * src[i - j * j];
        dst[i] = s;
    }
}

/* returns coefficient n of start * F^k, where F = E (odd=0) or O (odd=1);
   start == NULL means 1. If keep != NULL and keepk > 0, copies power keepk into keep. */
static seq_t power_coef(idx_t n, const seq_t *start, int odd, int k,
                        seq_t *keep, int keepk)
{
    seq_t *a = alloc_zero(n + 1);
    seq_t *b = alloc_zero(n + 1);
    seq_t *t;
    seq_t r;
    idx_t i;
    int s;
    if (start) {
        for (i = 0; i <= n; i++) a[i] = start[i];
    } else {
        a[0] = 1;
    }
    for (s = 1; s <= k; s++) {
        mul_sparse(b, a, n, odd);
        t = a; a = b; b = t;
        if (keep && s == keepk)
            for (i = 0; i <= n; i++) keep[i] = a[i];
    }
    r = a[n];
    release(a, n + 1);
    release(b, n + 1);
    return r;
}

seq_t a(idx_t n)
{
    seq_t e9, o9, mix;
    seq_t *o4 = alloc_zero(n + 1);
    e9 = power_coef(n, NULL, 0, 9, NULL, 0);
    o9 = power_coef(n, NULL, 1, 9, o4, 4);
    mix = power_coef(n, o4, 0, 5, NULL, 0);
    release(o4, n + 1);
    return e9 + 18 * mix + o9;
}
