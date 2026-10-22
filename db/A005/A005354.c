/*
 * A005354  Number of asymmetric planar trees with n nodes.
 * https://oeis.org/A005354
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, nice
 * Formula:   a(0) = a(1) = 1; for n >= 2, a(n) = A022553(n-1) - (A000108(n-1)
 *            + [n even]*A000108(n/2-1))/2, where A022553(m) = (1/(2m)) *
 *            Sum_{d|m} mu(m/d)*binomial(2d,d) and A000108(k) =
 *            binomial(2k,k)/(k+1). Exact multi-limb integer arithmetic, then
 *            converted to seq_t
 * Range:     u8 n <= 9, u16 n <= 14, u32 n <= 23, u64 n <= 40, u128 n <= 73,
 *            big any n
 * Helpers:   b_add, b_central, b_div, b_mul, b_sub, b_to_seq, b_trim
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>
#include <stddef.h>
#include "../utils/b_add.h"
#include "../utils/b_central.h"
#include "../utils/b_div.h"
#include "../utils/b_mul.h"
#include "../utils/b_sub.h"
#include "../utils/b_to_seq.h"
#include "../utils/b_trim.h"

static int mobius(idx_t k) {
    int mu = 1;
    idx_t p;
    for (p = 2; p * p <= k; p++) {
        if (k % p == 0) {
            k /= p;
            if (k % p == 0) return 0;
            mu = -mu;
        }
    }
    if (k > 1) mu = -mu;
    return mu;
}

seq_t a(idx_t n) {
    idx_t m, d;
    size_t cap, lp, ln, lt;
    uint32_t *pos, *neg, *t;
    seq_t r;

    if (n < 2) return 1;
    m = n - 1;
    cap = (size_t)(n / 16 + 8);
    pos = (uint32_t *)calloc(cap, sizeof(uint32_t));
    neg = (uint32_t *)calloc(cap, sizeof(uint32_t));
    t = (uint32_t *)calloc(cap, sizeof(uint32_t));
    lp = 1; ln = 1; lt = 1;

    for (d = 1; d <= m; d++) {
        int mu;
        if (m % d != 0) continue;
        mu = mobius(m / d);
        if (mu == 0) continue;
        b_central(t, &lt, d);
        if (mu > 0) b_add(pos, &lp, t, lt);
        else b_add(neg, &ln, t, lt);
    }
    b_sub(pos, &lp, neg, ln);
    b_div(pos, &lp, (uint32_t)(2 * m));          /* A022553(m) */

    b_central(t, &lt, m);
    b_div(t, &lt, (uint32_t)(m + 1));            /* A000108(n-1) */
    if (n % 2 == 0) {
        b_central(neg, &ln, n / 2 - 1);
        b_div(neg, &ln, (uint32_t)(n / 2));      /* A000108(n/2-1) */
        b_add(t, &lt, neg, ln);
    }
    b_div(t, &lt, 2);
    b_sub(pos, &lp, t, lt);

    r = b_to_seq(pos, lp);
    free(pos);
    free(neg);
    free(t);
    return r;
}
