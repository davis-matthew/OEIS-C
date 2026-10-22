/*
 * A007242  McKay-Thompson series of class 2a for the Monster group.
 * https://oeis.org/A007242
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign
 * Formula:   a(n) = [x^n] E6(x) / Product_{k>=1} (1-x^k)^12, where E6(x) = 1
 *            - 504*Sum_{k>=1} sigma_5(k)*x^k (i.e. T2a = E6/eta^12 =
 *            sqrt(j-1728)). P(x) = Product (1-x^k)^(-12) satisfies m*p(m) =
 *            12*Sum_{k=1..m} sigma(k)*p(m-k), p(0) = 1
 * Range:     i8 n <= 0, i16 n <= 2, i32 n <= 7, i64 n <= 28, i128 n <= 107,
 *            big any n
 * Helpers:   grow_zero, sigma5
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>
#include "../utils/grow_zero.h"
#include "../utils/sigma5.h"

static seq_t *P = NULL;
static uint64_t *S = NULL;
static idx_t cap = 0, have = 0;

static void ensure(idx_t n)
{
    idx_t m, k, d;
    if (n < have) return;
    if (n + 1 > cap) {
        idx_t nc = cap ? cap : 16;
        while (nc < n + 1) nc *= 2;
        P = grow_zero(P, cap, nc);
        S = (uint64_t *)realloc(S, (size_t)nc * sizeof(uint64_t));
        cap = nc;
    }
    for (m = have; m <= n; m++) {
        uint64_t s = 0;
        if (m > 0) {
            for (d = 1; d * d <= m; d++)
                if (m % d == 0) {
                    s += d;
                    if (d != m / d) s += m / d;
                }
        }
        S[m] = s;
        if (m == 0) {
            P[0] = 1;
        } else {
            seq_t acc = 0;
            for (k = 1; k <= m; k++)
                acc += P[m - k] * (idx_t)(12 * S[k]);
            P[m] = acc / (seq_t)m;
        }
    }
    have = n + 1;
}

seq_t a(idx_t n)
{
    seq_t res;
    idx_t k;
    ensure(n);
    res = P[n];
    for (k = 1; k <= n; k++) {
        seq_t c = (seq_t)504 * sigma5(k);
        res -= c * P[n - k];
    }
    return res;
}
