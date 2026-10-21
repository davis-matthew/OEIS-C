/*
 * A208850  Expansion of phi(q^2) / phi(-q) in powers of q where phi() is a
 *          Ramanujan theta function.
 * https://oeis.org/A208850
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = coefficient of φ(q²)/φ(–q) in the q‑expansion (φ is
 *            Ramanujan's theta function). The sequence is computed by series
 *            division: A(q) = φ(q²) = 1 + Σ_{m≥1} 2·q^{2m²} B(q) = φ(–q) = 1
 *            + Σ_{m≥1} 2·(–1)^m q^{m²} Since B[0] = 1, the ratio a = A / B
 *            satisfies a[n] = A[n] – Σ_{m=1}^{⌊√n⌋} B[m²] · a...
 * Range:     u8 n <= 8, u16 n <= 24, u32 n <= 76, u64 n <= 259, u128 n <= 932,
 *            big any n
 * Helpers:   mul_small
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>
#include "../utils/mul_small.h"

/* Compute integer square root (floor) for uint64_t values. */
static inline uint64_t isqrt_u64(uint64_t x)
{
    uint64_t r = 0;
    while ((r + 1) * (r + 1) <= x)
        ++r;
    return r;
}

static seq_t *cache = NULL;
static idx_t cache_size = 0;

seq_t a(idx_t n)
{
    /* Ensure the cache covers 0 … n */
    if (n >= cache_size) {
        idx_t new_size = n + 1;
        seq_t *new_cache = (seq_t *)realloc(cache, new_size * sizeof(seq_t));
        if (!new_cache) abort();            /* allocation failed */
        cache = new_cache;

        /* Compute missing terms */
        for (idx_t i = cache_size; i <= n; ++i) {
            seq_t term;

            /* A[i]: coefficient of φ(q²) */
            if (i == 0)
                term = 1;
            else if (i & 1)                 /* odd i → zero */
                term = 0;
            else { /* i even */
                uint64_t half = i >> 1;
                uint64_t t = isqrt_u64(half);
                term = (t * t == half) ? 2 : 0;
            }

            /* Sum contributions from B[m²]·a[i–m²] */
            seq_t neg_sum = 0;   /* m odd → B negative */
            seq_t pos_sum = 0;   /* m even → B positive */

            for (uint64_t m = 1; m * m <= i; ++m) {
                uint64_t k = m * m;
                seq_t prod = mul_small(cache[i - k], 2);
                if (m & 1)
                    neg_sum += prod;  /* add when B[m²] = –2 */
                else
                    pos_sum += prod;  /* subtract when B[m²] = +2 */
            }

            /* a[i] = term + neg_sum – pos_sum */
            term += neg_sum;         /* term = term + neg_sum */
            term -= pos_sum;         /* term = term – pos_sum */

            cache[i] = term;
        }
        cache_size = new_size;
    }
    return cache[n];
}
