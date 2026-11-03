/*
 * A064945  a(n) = Sum_{i|n, j|n, j >= i} i.
 * https://oeis.org/A064945
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = Sum_{i|n, j|n, j >= i} i
 * Range:     u8 n <= 47, u16 n <= 3359, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Validated: standard, pass, 79 terms through n = 8317 (2026-11-01)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

/* Comparator for qsort on idx_t array */
static int cmp_idx_t(const void *a, const void *b)
{
    idx_t v1 = *(const idx_t *)a;
    idx_t v2 = *(const idx_t *)b;
    return (v1 < v2) ? -1 : (v1 > v2) ? 1 : 0;
}

seq_t a(idx_t n)
{
    /* collect divisors of n */
    idx_t divs[256];          /* 256 divisors enough for n up to very large values */
    size_t cnt = 0;
    for (idx_t i = 1; i <= n / i; ++i) {
        if (n % i == 0) {
            divs[cnt++] = i;
            idx_t j = n / i;
            if (j != i) divs[cnt++] = j;
        }
    }

    /* sort the divisors ascending */
    qsort(divs, cnt, sizeof(idx_t), cmp_idx_t);

    /* compute the sum a(n) = Σ d_k * (τ(n) - k) where k is 0‑based rank */
    seq_t result = 0;
    for (size_t k = 0; k < cnt; ++k) {
        seq_t d     = (seq_t)divs[k];
        seq_t mult  = (seq_t)(cnt - k);   /* τ(n) - rank (rank = k+1) */
        result += d * mult;
    }
    return result;
}
