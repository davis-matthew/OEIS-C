/*
 * A203237  (n-1)-st elementary symmetric function of the first n terms of the
 *          upper Wythoff sequence, A001950.
 * https://oeis.org/A203237
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = elementary symmetric sum of degree (n‑1) of the first n
 *            terms * of the upper Wythoff sequence (Beatty sequence for ϕ² =
 *            ϕ+1), i.e. * a(n) = Σ_{1≤i1<…<i_{n-1}≤n} Π_{j=1}^{n-1} b_{ij}, *
 *            where b_k = ⌊k·ϕ²⌋ = ⌊k·ϕ⌋ + k
 * Range:     u8 n <= 3, u16 n <= 5, u32 n <= 9, u64 n <= 15, u128 n <= 17,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <math.h>
#include <stdint.h>

static const long double PHI = (1.0L + sqrtl(5.0L)) / 2.0L;   /* ϕ */

static inline seq_t mul_seq(seq_t x, seq_t y)
{
    /* Use an unsigned 128‑bit intermediate to avoid overflow on 64‑bit builds. */
    return (seq_t)((__uint128_t)x * y);
}

seq_t a(idx_t n)
{
    /* Off‑by‑one: we need e[0..n] (size n+1). */
    idx_t m = n + 1;
    seq_t *e = (seq_t *)malloc((size_t)m * sizeof(seq_t));
    if (e == NULL) return (seq_t)0;            /* allocation failed */

    for (idx_t i = 0; i < m; ++i) e[i] = 0;
    e[0] = (seq_t)1;                            /* e0 is always 1 */

    for (idx_t k = 1; k <= n; ++k)
    {
        /* upper Wythoff term: floor(k·ϕ²) = floor(k·ϕ)+k */
        long double tmp = (long double)k * PHI;
        unsigned long long lk = (unsigned long long)floorl(tmp);
        seq_t term = (seq_t)(lk + k);

        /* Update elementary symmetric sums from high to low. */
        for (idx_t j = k; j >= 1; --j)
        {
            e[j] += mul_seq(e[j - 1], term);
        }
    }

    seq_t result = e[n - 1];
    free(e);
    return result;
}
