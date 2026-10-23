/*
 * A037216  Expansion of ( Sum_{k>=0} k*q^(k^2) )^4.
 * https://oeis.org/A037216
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = coefficient of q^n in ( Σ_{k≥1} k·q^{k²} )⁴ where the sum
 *            is over positive integers k
 * Range:     u8 n <= 27, u16 n <= 149, u32 n <= 5669, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>

/* Return ⌊√n⌋ for n ≤ 2^63‑1.  Uses bit‑wise integer square‑root. */
static inline idx_t floor_sqrt_u64(idx_t n)
{
    idx_t r = 0, bit = 1ULL << 62;   /* start with the largest power of 4 ≤ n */
    while (bit > n) bit >>= 2;
    while (bit) {
        if (n >= r + bit) {
            n -= r + bit;
            r = (r >> 1) + bit;
        } else
            r >>= 1;
        bit >>= 2;
    }
    return r;
}

/* Compute the sequence up to index n and return a(n).  Naïve convolutions
   are used; this is fast enough for n ≤ 10 000. */
seq_t a(idx_t n)
{
    idx_t m = floor_sqrt_u64(n);
    idx_t size = (size_t)n + 1;
    seq_t *f = (seq_t *)calloc(size, sizeof(seq_t));  /* f[s] = k if s = k² */
    seq_t *g = (seq_t *)calloc(size, sizeof(seq_t));  /* f * f */
    seq_t *h = (seq_t *)calloc(size, sizeof(seq_t));  /* g * f */
    seq_t *res = (seq_t *)calloc(size, sizeof(seq_t));/* h * f */

    /* f(s) = k for s = k² (k ≥ 1) */
    for (idx_t k = 1; k <= m; ++k)
        f[k * k] = (seq_t)k;

    /* g = f * f: sum over k₁² + k₂² = s of k₁·k₂ */
    for (idx_t k1 = 1; k1 <= m; ++k1) {
        idx_t i = k1 * k1;
        for (idx_t k2 = 1; k2 <= m; ++k2) {
            idx_t s = i + k2 * k2;
            if (s > n) break;                      /* k₂ grows, so we can stop */
            g[s] += (seq_t)k1 * (seq_t)k2;
        }
    }

    /* h = g * f: sum over s₁ + k² = s of g[s₁]·k */
    for (idx_t s1 = 0; s1 <= n; ++s1) if (g[s1]) {
        for (idx_t k = 1; k <= m; ++k) {
            idx_t s = s1 + k * k;
            if (s > n) break;
            h[s] += g[s1] * (seq_t)k;
        }
    }

    /* res = h * f: final convolution yielding a(s) */
    for (idx_t s1 = 0; s1 <= n; ++s1) if (h[s1]) {
        for (idx_t k = 1; k <= m; ++k) {
            idx_t s = s1 + k * k;
            if (s > n) break;
            res[s] += h[s1] * (seq_t)k;
        }
    }

    seq_t answer = res[n];
    free(f); free(g); free(h); free(res);
    return answer;
}
