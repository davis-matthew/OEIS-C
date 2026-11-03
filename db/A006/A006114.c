/*
 * A006114  Gaussian binomial coefficient [ 2n,n ] for q=5.
 * https://oeis.org/A006114
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Product_{i=1..n} (5^(n+i)-1)/(5^i-1), i.e. q-binomial
 *            [2n,n] at q=5; fixed-width builds use the q-Pascal rule [m,k] =
 *            [m-1,k-1] + 5^k*[m-1,k], [m,0] = 1
 * Range:     u8 n <= 1, u16 n <= 2, u32 n <= 3, u64 n <= 5, u128 n <= 7,
 *            big any n
 * Validated: standard, pass, 26 terms through n = 36 (2026-11-03)
 * Helpers:   mem
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdbool.h>
#include <stdint.h>
#include "../utils/mem.h"

static bool seq_is_fixed_width(void)
{
    seq_t t = 1;
    int i;
    for (i = 0; i < 256; i++) {
        t *= 2;
        if (t == 0)
            return true;
    }
    return false;
}

static seq_t pow5(idx_t e) {
    seq_t result = 1;
    seq_t base = 5;
    while (e > 0) {
        if (e & 1) result *= base;
        base *= base;
        e >>= 1;
    }
    return result;
}

seq_t a(idx_t n) {
    if (n == 0) return 1;

    if (!seq_is_fixed_width()) {
        /* partial products are q-binomials [n+k,k], so each division is exact */
        seq_t result = 1;
        seq_t qi = 1;
        seq_t qn = pow5(n);
        for (idx_t i = 1; i <= n; i++) {
            qi *= 5;
            result = result * (qn * qi - 1) / (qi - 1);
        }
        return result;
    }

    /* fixed width: only + and *, so the result is exact whenever it fits */
    seq_t *c = mem_new(seq_t, (size_t)(n + 1));
    seq_t *pw = mem_new(seq_t, (size_t)(n + 1));
    idx_t k, m;
    pw[0] = 1;
    for (k = 1; k <= n; k++) pw[k] = pw[k - 1] * 5;
    c[0] = 1;
    for (k = 1; k <= n; k++) c[k] = 0;
    for (m = 1; m <= 2 * n; m++) {
        idx_t top = m < n ? m : n;
        for (k = top; k >= 1; k--)
            c[k] = c[k - 1] + pw[k] * c[k];
    }
    seq_t result = c[n];
    mem_free(c);
    mem_free(pw);
    return result;
}
