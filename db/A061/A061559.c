/*
 * A061559  Array read by antidiagonals: T(k,d) = number of different
 *          hyperplanes in d-space with integer coefficients in set
 *          {-k,...,-1,0,1,...,k}.
 * https://oeis.org/A061559
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, tabl
 * Formula:   a(n) = T(k,d) where (k,d) is the pair obtained by reading the *
 *            array of numbers T(k,d) of OEIS A061559 by antidiagonals. *
 *            T(k,d) is computed by * T(k,d) = (1/2) * Σ_{j=1}^{d+1}
 *            binomial(d+1,j) * 2^j * M(j,k) - 1 * where M(m,k) = Σ_{t=1}^{k}
 *            μ(t) * ⌊k/t⌋^m * μ(t) is the Möbius function. The func...
 * Range:     u8 n <= 7, u16 n <= 25, u32 n <= 43, u64 n <= 43, u128 n <= 43,
 *            big any n
 * Validated: partial, pass, 43 terms through n = 43 (2026-11-01)
 * Helpers:   binomial, ipow_i32
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <math.h>
#include <stdint.h>
#include <stdbool.h>
#include <stddef.h>
#include <stdlib.h>
#include "../utils/binomial.h"
#include "../utils/ipow_i32.h"

/* Möbius function for the divisor in the loop */
static inline int mu_of(idx_t d) {
    int n = (int)d;
    int sign = 1;
    int i = 2;
    while ((int64_t)i * i <= n) {
        if (n % i == 0) {
            int cnt = 0;
            while (n % i == 0) {
                n /= i;
                ++cnt;
                if (cnt > 1) return 0;        /* square factor */
            }
            sign = -sign;
        }
        ++i;
    }
    if (n > 1) sign = -sign;
    return sign;
}

#define MAX_SZ 256  /* enough for n up to about 10^5 */

/* memoised tables for M(j,k) and T(k,d) */
static seq_t M_cache[MAX_SZ][MAX_SZ] = {0};
static seq_t T_cache[MAX_SZ][MAX_SZ] = {0};

/* return M(m,k) for 1 <= m,k < MAX_SZ */
static seq_t get_M(int m, int k)
{
    if (M_cache[m][k]) return M_cache[m][k];

    seq_t pos = 0, neg = 0;
    for (int t = 1; t <= k; ++t) {
        int mu = mu_of((idx_t)t);   /* Möbius function from utils */
        if (mu == 0) continue;

        int div = k / t;
        seq_t pow = ipow_i32((seq_t)div, m);
        if (mu > 0) pos += pow;
        else        neg += pow;
    }
    seq_t result = pos - neg;
    M_cache[m][k] = result;
    return result;
}

/* return T(k,d) for 1 <= k,d < MAX_SZ */
static seq_t get_T(int k, int d)
{
    if (T_cache[k][d]) return T_cache[k][d];

    int n = d + 1;
    seq_t sum = 0;
    for (int j = 1; j <= n; ++j) {
        seq_t bin = binomial((idx_t)n, (idx_t)j);
        seq_t pow2 = ipow_i32((seq_t)2, j);
        seq_t Mval = get_M(j, k);
        seq_t term = bin;
        term *= pow2;
        term *= Mval;
        sum += term;
    }
    seq_t res = sum / 2;
    res -= 1;
    T_cache[k][d] = res;
    return res;
}

/* map index n (1‑based) to pair (k,d) in antidiagonal order
 * and return the corresponding T(k,d). */
seq_t a(idx_t n)
{
    if (n == 0) return 0;   /* undefined for offset 1, but safe */

    /* find the diagonal s so that C(s) = (s-1)*s/2 >= n */
    double disc = sqrt((double)(8.0 * (double)n + 1.0));
    idx_t s = (idx_t)((1.0 + disc) / 2.0);
    if (s * (s - 1) / 2 < n) ++s;   /* ensure C(s) >= n */

    idx_t prevC = (s - 1) * (s - 2) / 2;
    idx_t offset = n - prevC - 1;   /* 0‑based position in diagonal */

    int k = (int)(s - 1 - offset);
    int d = (int)(offset + 1);

    return get_T(k, d);
}
