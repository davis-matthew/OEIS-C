/*
 * A119331  Number triangle T(n,k)=sum{i=0..n, (-1)^(n-i)*C(n,i)*sum{j=0..i-k,
 *          C(k,2j)*C(i-k,2j)*2^j}}.
 * https://oeis.org/A119331
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, sign, tabl
 * Formula:   a(n) = T(r,c) where r = ⌊(√(8n+1)-1)/2⌋ (largest integer with
 *            r(r+1)/2 ≤ n), c = n - r(r+1)/2. T(n,k) = Σ_{i=k..n} (-1)^(n-i)
 *            C(n,i) * Σ_{j=0..min(⌊(i-k)/2⌋, ⌊k/2⌋)} C(k,2j) C(i-k,2j) 2^j (C
 *            are binomial coefficients)
 * Range:     i8 n <= 35, i16 n <= 65, i32 n <= 65, i64 n <= 65, i128 n <= 65,
 *            big any n
 * Helpers:   binom, pow2
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include "../utils/binom.h"   /* provides seq_t binom(idx_t a, idx_t b) */
#include "../utils/pow2.h"    /* provides seq_t pow2(idx_t exp) */

static seq_t compute_T(idx_t n, idx_t k)
{
    seq_t total = 0;
    for (idx_t i = k; i <= n; ++i) {
        idx_t m = i - k;
        seq_t inner = 0;
        for (idx_t j = 0; ; ++j) {
            idx_t twoj = j << 1;
            if (twoj > m || twoj > k)
                break;
            seq_t term = binom(k, twoj);
            term = term * binom(m, twoj);
            term = term * pow2(j);
            inner += term;
        }
        seq_t coeff = binom(n, i);
        if (((n - i) & 1) != 0)
            coeff = -coeff;
        total += coeff * inner;
    }
    return total;
}

seq_t a(idx_t n)
{
    /* find row r such that r(r+1)/2 ≤ n < (r+1)(r+2)/2 */
    idx_t r = 0;
    while (((r + 1) * (r + 2)) / 2 <= n)
        ++r;
    idx_t c = n - (r * (r + 1)) / 2;
    return compute_T(r, c);
}
