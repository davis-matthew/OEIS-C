/*
 * A007714  Number of 5-level rooted trees with n leaves.
 * https://oeis.org/A007714
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a = EULER(EULER(EULER(EULER(1,1,1,...)))), the Euler transform
 *            applied 4 times to the all-1's sequence; b = EULER(c) means
 *            Product_{k>=1} 1/(1-x^k)^c(k) = Sum_{n>=0} b(n) x^n
 * Range:     u8 n <= 5, u16 n <= 10, u32 n <= 20, u64 n <= 41, u128 n <= 85,
 *            big any n
 * Helpers:   a007714_gcd, alloc_zero, release, seq_is_fixed_width
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>
#include "../utils/alloc_zero.h"
#include "../utils/release.h"
#include "../utils/seq_is_fixed_width.h"
#include "../utils/a007714_gcd.h"

/* Exact-arithmetic Euler transform: n*b(n) = Sum_{k=1..n} d(k) b(n-k), d(k) = Sum_{e|k} e*c(e). */
static void a007714_euler_div(const seq_t *c, seq_t *b, idx_t len) {
    seq_t *d = alloc_zero(len);
    for (idx_t k = 1; k < len; k++)
        for (idx_t m = k; m < len; m += k)
            d[m] += (seq_t)k * c[k];
    b[0] = 1;
    for (idx_t i = 1; i < len; i++) {
        seq_t s = 0;
        for (idx_t k = 1; k <= i; k++)
            s += d[k] * b[i - k];
        b[i] = s / (seq_t)i;
    }
    release(d, len);
}

/* Division-free (overflow-safe) Euler transform: multiply by (1-x^k)^(-c(k))
   using multichoose coefficients C(c+j-1, j), each built without overflow. */
static void a007714_euler_mc(const seq_t *c, seq_t *b, idx_t len) {
    seq_t *mc = alloc_zero(len);
    b[0] = 1;
    for (idx_t k = 1; k < len; k++) {
        idx_t jmax = (len - 1) / k;
        mc[0] = 1;
        for (idx_t j = 1; j <= jmax; j++) {
            seq_t r = mc[j - 1];
            seq_t jj = (seq_t)j;
            seq_t g = a007714_gcd(r, jj);
            mc[j] = (r / g) * ((c[k] + (seq_t)(j - 1)) / (jj / g));
        }
        for (idx_t m = len - 1; m >= k; m--) {
            seq_t s = b[m];
            for (idx_t j = 1; j * k <= m; j++)
                s += mc[j] * b[m - j * k];
            b[m] = s;
        }
    }
    release(mc, len);
}

seq_t a(idx_t n) {
    static seq_t *memo = NULL;
    static idx_t memo_len = 0;
    bool fixed = seq_is_fixed_width();

    /* a(n) > 2^128 for n > 150, so no fixed-width type can hold it */
    if (fixed && n > 150)
        return 0;
    if (memo != NULL && n < memo_len)
        return memo[n];

    idx_t len = fixed ? (idx_t)151 : n + 1;
    if (len < memo_len)
        len = memo_len;

    seq_t *cur = alloc_zero(len);
    for (idx_t i = 0; i < len; i++)
        cur[i] = 1;

    for (int level = 0; level < 4; level++) {
        seq_t *nxt = alloc_zero(len);
        if (fixed)
            a007714_euler_mc(cur, nxt, len);
        else
            a007714_euler_div(cur, nxt, len);
        release(cur, len);
        cur = nxt;
    }

    if (memo != NULL)
        release(memo, memo_len);
    memo = cur;
    memo_len = len;
    return memo[n];
}
