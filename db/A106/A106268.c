/*
 * A106268  Number triangle T(n,k) = (-1)^(n-k)*binomial(k-n, n-k) = (0^(n-k)
 *          + binomial(2*(n-k), n-k))/2 if k <= n, 0 otherwise; Riordan array
 *          (1/(2-C(x)), x) where C(x) is g.f. for Catalan numbers A000108.
 * https://oeis.org/A106268
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, nonn, tabl
 * Formula:   a(n) = T(r,c) where * r = floor((sqrt(8*n+1)-1)/2) (row index) *
 *            c = n - r*(r+1)/2 (column index) * T(r,c) = 1 if r == c * T(r,c)
 *            = binomial(2*(r-c), r-c)/2 otherwise * * The triangle T is
 *            defined by * T(n,k) = (-1)^(n-k) * binomial(k-n, n-k) for k <=
 *            n, 0 otherwise. * This code computes the flat...
 * Range:     u8 n <= 20, u16 n <= 54, u32 n <= 170, u64 n <= 629,
 *            u128 n <= 1325, big any n
 * Validated: standard, pass, 82 terms through n = 1310 (2026-11-03)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

/* the code below runs in wide_t: 128 bits at the fixed widths (never narrower
   than seq_t), seq_t itself for big, so values that pass 2^64 on the way to a
   term that fits stay exact; a() returns the term in seq_t */
#ifdef __cplusplus
typedef seq_t wide_t;
#else
typedef __typeof__((seq_t)0 + (__int128)0) wide_t;
#endif
#define seq_t wide_t

/* Compute a_wide(n) */
seq_t a_wide(idx_t n)
{
    /* Find row r: r*(r+1)/2 <= n < (r+1)*(r+2)/2          */
    idx_t r = 0;
    while (((r + 1) * (r + 2)) / 2 <= n)
        ++r;

    /* Column c inside the found row */
    idx_t c = n - (r * (r + 1)) / 2;

    /* m = r - c (non‑negative) */
    idx_t m = r - c;

    if (m == 0) return 1;               /* T(r,r) = 1 */

    /* Compute binomial(2*m, m) with a multiplicative approach */
    seq_t val = 1;
    for (idx_t i = 1; i <= m; ++i) {
        val = val * (m + i);            /* numerator term (m+i)          */
        val = val / i;                  /* divide by i (exact division)   */
    }

    /* Divide by 2, always exact */
    val = val / 2;
    return val;
}

#undef seq_t
seq_t a(idx_t n)
{
    return (seq_t)a_wide(n);
}
