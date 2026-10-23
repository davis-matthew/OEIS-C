/*
 * A100257  Triangle of expansions of 2^(k-1)*x^k in terms of T(n,x), in
 *          descending degrees n of T, with T the Chebyshev polynomials.
 * https://oeis.org/A100257
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, tabl
 * Formula:   a(n) = (C(k, (k-m)/2) + (m>0) * C(k, (k+m)/2)) / 2 where k =
 *            max{r | r(r+1)/2 ≤ n}, m = k - (n - k(k+1)/2)
 * Range:     u8 n <= 62, u16 n <= 205, u32 n <= 663, u64 n <= 2407,
 *            u128 n <= 6104, big any n
 * Helpers:   int_sqrt, my_binom
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Helper to compute binomial coefficients accurately on 64‑bit builds
   using 128‑bit arithmetic to avoid overflow. */
#include "../utils/int_sqrt.h"
#include "../utils/my_binom.h"

seq_t a(idx_t n)
{
    /* first term */
    if (n == 0) return (seq_t)1;

    /* find row k: largest k with k(k+1)/2 ≤ n */
    idx_t k;
    {
        /* k ≈ floor((sqrt(8n+1)-1)/2) */
        idx_t m = (n << 3) + 1;             /* 8n + 1 */
        idx_t s = int_sqrt(m);              /* floor sqrt */
        k = (s - 1) >> 1;                   /* (s-1)/2 */

        /* correct possible off‑by‑one errors */
        while (((k + 1) * (k + 2)) / 2 <= n) ++k;
        while ((k * (k + 1)) / 2 > n) --k;
    }

    idx_t base = (k * (k + 1)) / 2;            /* start index of row k */
    idx_t offset = n - base;                   /* position inside row k */
    idx_t m = k - offset;                      /* degree of T in this term */

    /* parity check – coefficient is zero when k and m differ in parity */
    if (((k - m) & 1ULL) != 0ULL) return (seq_t)0;

    /* binomial indices */
    idx_t j1 = (k - m) >> 1;
    idx_t j2 = (k + m) >> 1;

    seq_t c1 = my_binom(k, j1);
    seq_t sum = c1;
    if (m != 0) {
        seq_t c2 = my_binom(k, j2);
        sum += c2;
    }
    return sum / (seq_t)2;     /* division by 2 is exact */
}
