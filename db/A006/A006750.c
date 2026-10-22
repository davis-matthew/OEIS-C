/*
 * A006750  Coefficient of P_5(x) in the Legendre expansion of x^n.
 * https://oeis.org/A006750
 *
 * Offset:    2    (a(2) is the first term)
 * Keywords:  nonn, changed
 * Formula:   a(n) = 44*(n-1)*n*C(2N,N) / ((2n+3)*(2n+5)*(2n+7)*2^s(N)), where
 *            N = 2n+1 and s(N) = number of 1's in binary N. This is the
 *            coefficient of P_5(x) in x^N = Sum c_k P_k(x), multiplied by the
 *            reduced denominator C(2N,N)/2^s(N) of the leading coefficient
 *            2^N/C(2N,N). It is computed from the prime fac...
 * Range:     u8 n <= 3, u16 n <= 5, u32 n <= 9, u64 n <= 17, u128 n <= 33,
 *            big any n
 * Helpers:   a006750_vbinom, a006750_vp
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include "../utils/a006750_vbinom.h"
#include "../utils/a006750_vp.h"

static unsigned char *a006750_comp = NULL;
static uint64_t a006750_lim = 0;

static void a006750_sieve(uint64_t L)
{
    uint64_t i, j;
    if (L <= a006750_lim) return;
    free(a006750_comp);
    a006750_comp = (unsigned char *)malloc((size_t)(L + 1));
    memset(a006750_comp, 0, (size_t)(L + 1));
    a006750_comp[0] = 1;
    a006750_comp[1] = 1;
    for (i = 2; i * i <= L; i++)
        if (!a006750_comp[i])
            for (j = i * i; j <= L; j += i) a006750_comp[j] = 1;
    a006750_lim = L;
}

seq_t a(idx_t n)
{
    uint64_t m = (uint64_t)n;
    uint64_t N = 2 * m + 1;
    uint64_t L = 4 * m + 8;
    uint64_t p, t;
    int s = 0;
    seq_t num = 1, den = 1;
    long long e, k;

    for (t = N; t; t >>= 1) s += (int)(t & 1);
    a006750_sieve(L);

    for (p = 2; p <= L; p++) {
        if (a006750_comp[p]) continue;
        e = a006750_vp(44, p) + a006750_vp(m - 1, p) + a006750_vp(m, p)
          + a006750_vbinom(N, p)
          - a006750_vp(2 * m + 3, p) - a006750_vp(2 * m + 5, p) - a006750_vp(2 * m + 7, p);
        if (p == 2) e -= s;
        if (e > 0) {
            for (k = 0; k < e; k++) num *= (seq_t)p;
        } else if (e < 0) {
            for (k = 0; k < -e; k++) den *= (seq_t)p;
        }
    }
    return num / den;
}
