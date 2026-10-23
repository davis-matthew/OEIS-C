/*
 * A046775  Number of partitions of n with equal number of parts congruent to
 *          each of 1, 2, 3 and 4 (mod 5).
 * https://oeis.org/A046775
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 34, u16 n <= 79, u32 n <= 199, u64 n <= 559,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  OEIS A046775 – Number of partitions of n with equal number of parts
    congruent to 1,2,3 and 4 (mod 5).
    The generating function (in terms of x) is

        G(x) = (Σ_{k≥0} x^(10k) / Π_{j=1..k} (1 - x^(5j))^3)
/                Π_{m≥1} (1 - x^(5m)) .
    In terms of y = x^5 this becomes

        G(y) = Σ_{k≥0} y^(2k) · H_k(y) · A(y)

    where
        H_k(y) = 1 / Π_{j=1..k} (1 - y^j)^3
        A(y)   = 1 / Π_{m≥1} (1 - y^m)   (a classical partition generating function).

    The coefficient of y^t ( = a(5t) ) is obtained by
    1) computing the partition numbers p(m) for 0 ≤ m ≤ t,
    2) building the series S(y)=Σ_{k} y^(2k)·H_k(y) up to degree t by successive
       convolution with the series (1 - y^j)^(-3),
    3) finally convolving S with A (i.e. with p(m)) to obtain the desired value.
*/

#include <stdlib.h>
#include <stdint.h>
#include <string.h>

/* Helper: use 128‑bit intermediate for products when available. */
#if defined(__SIZEOF_INT128__)
typedef unsigned __int128 uint128_t;
#endif

/*--------------------------------------------------------------------*/
/*  Return the OEIS sequence value a(n);  n≥0.  */
seq_t a(idx_t n)
{
    /* a(n) is zero unless n is a multiple of 5. */
    if (n % 5 != 0)
        return 0;
    idx_t t = n / 5;                /* work in variable y = x^5  */

    /* ---- 1. Partition numbers p[0..t] ----- */
    seq_t *p = (seq_t*)calloc(t + 1, sizeof(seq_t));
    if (!p) return 0;
    p[0] = 1;
    for (idx_t i = 1; i <= t; ++i)
        for (idx_t j = i; j <= t; ++j)
        {
            seq_t add = p[j - i];
#if defined(__SIZEOF_INT128__)
            uint128_t sum = (uint128_t)p[j] + (uint128_t)add;
            p[j] = (seq_t)sum;
#else
            p[j] += add;
#endif
        }

    /* ---- 2. Series S(y) = Σ_k y^(2k)*H_k(y) ----- */
    seq_t *S = (seq_t*)calloc(t + 1, sizeof(seq_t));
    if (!S) { free(p); return 0; }

    /* current H_k series stored in h[0..t]  (H_0 = 1) */
    seq_t *h = (seq_t*)calloc(t + 1, sizeof(seq_t));
    if (!h) { free(p); free(S); return 0; }
    h[0] = 1;                       /* H_0 = 1 */

    idx_t maxK = t / 2;              /* largest k such that 2k ≤ t */
    for (idx_t k = 0; k <= maxK; ++k)
    {
        idx_t shift = 2 * k;
        /* add current H_k shifted by 2k to S */
        for (idx_t u = 0; u + shift <= t; ++u)
        {
            seq_t add = h[u];
#if defined(__SIZEOF_INT128__)
            uint128_t sum = (uint128_t)S[u + shift] + (uint128_t)add;
            S[u + shift] = (seq_t)sum;
#else
            S[u + shift] += add;
#endif
        }

        if (k == maxK) break;      /* no higher k needed */

        /* compute H_{k+1} = H_k * (1 - y^{k+1})^{-3}  */
        idx_t step = k + 1;          /* exponent step in y */
        seq_t *newh = (seq_t*)calloc(t + 1, sizeof(seq_t));
        if (!newh) { free(p); free(S); free(h); return 0; }

        for (idx_t u = 0; u <= t; ++u)
        {
            if (!h[u]) continue;
            seq_t base = h[u];
            idx_t maxd = (t - u) / step;

            for (idx_t d = 0; d <= maxd; ++d)
            {
                /* coefficient for d extra multiples of 5:
                   number of solutions to d1+d2+d3+d4 = d   = C(d+3,3)   */
                seq_t coeff = ((seq_t)(d + 3) *
                               (seq_t)(d + 2) *
                               (seq_t)(d + 1)) / 6;
                seq_t prod;
#if defined(__SIZEOF_INT128__)
                uint128_t mult = (uint128_t)base * (uint128_t)coeff;
                prod = (seq_t)mult;
#else
                prod = base * coeff;
#endif
                idx_t idx = u + step * d;
#if defined(__SIZEOF_INT128__)
                uint128_t sum = (uint128_t)newh[idx] + (uint128_t)prod;
                newh[idx] = (seq_t)sum;
#else
                newh[idx] += prod;
#endif
            }
        }
        free(h);
        h = newh;
    }
    free(h);        /* no longer needed */

    /* ---- 3. Convolve S with partition numbers p to get final result. */
    seq_t result = 0;
    for (idx_t u = 0; u <= t; ++u)
    {
        seq_t term = S[u];
        if (!term) continue;
        idx_t v = t - u;
        seq_t part = p[v];
        if (!part) continue;
        seq_t mult;
#if defined(__SIZEOF_INT128__)
        uint128_t prod = (uint128_t)term * (uint128_t)part;
        mult = (seq_t)prod;
#else
        mult = term * part;
#endif
#if defined(__SIZEOF_INT128__)
        uint128_t sum = (uint128_t)result + (uint128_t)mult;
        result = (seq_t)sum;
#else
        result += mult;
#endif
    }

    free(S);
    free(p);
    return result;
}
