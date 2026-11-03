/*
 * A046779  Number of partitions of n with equal nonzero number of parts
 *          congruent to each of 0, 1 and 3 (mod 4).
 * https://oeis.org/A046779
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = number of partitions of n with equal nonzero number of
 *            parts congruent to each of 0, 1 and 3 (mod 4). Generating
 *            function: G(x) = (∑_{k>0} x^(8k) / ∏_{j=1..k} (1 - x^(4j))^3) /
 *            ∏_{j≥0} (1 - x^(4j+2))
 * Range:     u8 n <= 31, u16 n <= 69, u32 n <= 179, u64 n <= 521,
 *            u128 n <= 1000, big any n
 * Validated: standard, pass, 85 terms through n = 935 (2026-11-03)
 * Helpers:   mem
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdlib.h>
#include <stdint.h>
#include "../utils/mem.h"

seq_t a(idx_t n)
{
    size_t N = (size_t)n;          /* maximum degree of the polynomials */
    size_t kmax = N / 8;           /* largest k with 8k ≤ n */

    /* allocate temporary arrays (calloc gives zeroed memory) */
    seq_t *poly_prod = mem_new(seq_t, N + 1);
    seq_t *tmp       = mem_new(seq_t, N + 1);
    seq_t *total     = mem_new(seq_t, N + 1);
    seq_t *g2        = mem_new(seq_t, N + 1);
    if (!poly_prod || !tmp || !total || !g2) {
        mem_free(poly_prod); mem_free(tmp); mem_free(total); mem_free(g2);
        return 0;
    }

    /* 1. partitions of n into parts ≡ 2 (mod 4) */
    g2[0] = 1;
    for (idx_t part = 2; part <= n; part += 4) {
        for (idx_t sum = part; sum <= n; ++sum)
            g2[sum] += g2[sum - part];
    }

    poly_prod[0] = 1;              /* product over an empty set */

    /* 2. accumulate the sum ∑_{k≥1} x^(8k)∏_{j=1..k}(1-x^(4j))^-3 */
    for (idx_t k = 1; k <= kmax; ++k) {
        idx_t m = 4 * k;           /* step of the new factor */

        /* copy current product to temporary buffer */
        for (size_t i = 0; i <= N; ++i)
            tmp[i] = poly_prod[i];

        /* convolve tmp with (1 - x^m)^-3, whose coefficients are binom(c+2,2) */
        for (idx_t i = 0; i <= N; ++i) {
            if (poly_prod[i] == 0) continue;
            seq_t base = poly_prod[i];
            for (idx_t c = 1; ; ++c) {
                idx_t idx = i + c * m;
                if (idx > N) break;
                seq_t coeff = (seq_t)(((c + 2) * (c + 1)) / 2);
                tmp[idx] += base * coeff;
            }
        }

        /* update the running product */
        for (size_t i = 0; i <= N; ++i)
            poly_prod[i] = tmp[i];

        /* add x^(8k) * poly_prod to the total polynomial */
        idx_t shift = 8 * k;
        for (idx_t i = 0; i + shift <= n; ++i)
            total[i + shift] += poly_prod[i];
    }

    /* 3. final convolution with the 2‑mod‑4 partitions */
    seq_t ans = 0;
    for (idx_t i = 0; i <= n; ++i) {
        if (total[i] == 0) continue;
        ans += total[i] * g2[n - i];
    }

    /* free allocations */
    mem_free(poly_prod);
    mem_free(tmp);
    mem_free(total);
    mem_free(g2);

    return ans;
}
