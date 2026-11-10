/*
 * A092987  A092985(n) divided by {A057237(n)}!.
 * https://oeis.org/A092987
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = A092985(n)/A057237(n)!, A092985(n) = Product_{k=1..n}
 *            (1+(k-1)*n), A057237(n) = (least prime factor of n) - 1 for n >=
 *            2, A057237(1) = 1
 * Range:     u8 n <= 3, u16 n <= 5, u32 n <= 7, u64 n <= 11, u128 n <= 15,
 *            big any n
 * Validated: partial, pass, 15 terms through n = 15 (2026-11-07)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>

static idx_t gcd_small_idx(idx_t a, idx_t b)
{
    while (b) { idx_t t = a % b; a = b; b = t; }
    return a;
}

seq_t a(idx_t n)
{
    idx_t m;
    if (n <= 1) m = 1;
    else {
        idx_t p = n;
        if (n % 2 == 0) p = 2;
        else {
            for (idx_t d = 3; d <= n / d; d += 2) {
                if (n % d == 0) { p = d; break; }
            }
        }
        m = p - 1;
    }
    idx_t *dv = (idx_t *)malloc(sizeof(idx_t) * (size_t)(m + 2));
    for (idx_t j = 0; j <= m; ++j) dv[j] = j;
    seq_t res = 1;
    for (idx_t k = 1; k <= n; ++k) {
        seq_t f = 1 + (seq_t)(k - 1) * (seq_t)n;
        for (idx_t j = 2; j <= m; ++j) {
            if (dv[j] == 1) continue;
            idx_t r = (idx_t)(f % (seq_t)dv[j]);
            idx_t g = gcd_small_idx(dv[j], r);
            if (g > 1) {
                f /= (seq_t)g;
                dv[j] /= g;
            }
        }
        res *= f;
    }
    for (idx_t j = 2; j <= m; ++j) {
        if (dv[j] > 1) res /= (seq_t)dv[j];
    }
    free(dv);
    return res;
}
