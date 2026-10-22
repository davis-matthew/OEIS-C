/*
 * A000175  Related to zeros of Bessel function.
 * https://oeis.org/A000175
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = t(n) * D(n), where t(1) = 1, t(n) = (1/n) *
 *            Sum_{k=1..n-1} t(k)*t(n-k) (t(n) = 4^n * sigma_n(0), the
 *            Rayleigh function at nu = 0) and D(n) = Prod_{d=1..n}
 *            d^floor(n/d). In integers: a(n) = Sum_{k=1..n-1}
 *            a(k)*a(n-k)*c(n,k), a(1) = 1, where c(n,k) =
 *            D(n)/(n*D(k)*D(n-k)) = Prod_{2<=d<n, (k m...
 * Range:     u8 n <= 5, u16 n <= 7, u32 n <= 11, u64 n <= 15, u128 n <= 23,
 *            big any n
 * Helpers:   a175_c
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stddef.h>
#include "../utils/a175_c.h"

#define A175_MAX 10002

static seq_t a175_b[A175_MAX];
static idx_t a175_have = 0;

seq_t a(idx_t n)
{
    idx_t i, k;
    if (n < 1 || n >= A175_MAX)
        return 0;
    if (a175_have == 0) {
        a175_b[1] = 1;
        a175_have = 1;
    }
    for (i = a175_have + 1; i <= n; i++) {
        seq_t s = 0;
        for (k = 1; 2 * k < i; k++) {
            seq_t t = a175_b[k] * a175_b[i - k];
            t *= a175_c(i, k);
            s += t + t;
        }
        if (i % 2 == 0) {
            seq_t t = a175_b[i / 2] * a175_b[i / 2];
            t *= a175_c(i, i / 2);
            s += t;
        }
        a175_b[i] = s;
        a175_have = i;
    }
    return a175_b[n];
}
