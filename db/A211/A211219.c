/*
 * A211219  Maximum value of sigma(x) * sigma(y) * sigma(z), where x + y + z =
 *          n.
 * https://oeis.org/A211219
 *
 * Offset:    3    (a(3) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = max_{x+y+z=n, x,y,z≥1} sigma(x)*sigma(y)*sigma(z)
 * Range:     u8 n <= 11, u16 n <= 55, u32 n <= 500, u64 n <= 500,
 *            u128 n <= 500, big any n
 * Validated: standard, pass, 60 terms through n = 479 (2026-11-03)
 * Helpers:   mem
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* sigma is the sum–of–divisors function */

#include <stdlib.h>   /* calloc, free */
#include <stdint.h>
#include <stddef.h>
#include "../utils/mem.h"

static seq_t maximum_product(idx_t n)
{
    /* allocate arrays of length n+1 (index 0 unused) */
    size_t N = (size_t)n + 1;
    seq_t *sigma = mem_new(seq_t, N);
    if (!sigma) return 0;                 /* OOM guard */

    /* compute sigma[1..n] by sieve O(n log n) */
    for (idx_t i = 1; i <= n; ++i) {
        for (idx_t j = i; j <= n; j += i) {
            sigma[j] += (seq_t)i;
        }
    }

    /* array that stores the maximum product of two sigma values
       for a given sum (pair_best[s] = max_{1≤i<s} sigma[i]*sigma[s-i]) */
    seq_t *pair_best = mem_new(seq_t, N);
    if (!pair_best) { mem_free(sigma); return 0; }

    for (idx_t s = 2; s <= n; ++s) {
        seq_t best = 0;
        for (idx_t i = 1; i < s; ++i) {
            seq_t prod = sigma[i] * sigma[s - i];
            if (prod > best) best = prod;
        }
        pair_best[s] = best;
    }

    /* compute the answer from optimal decomposition x+(n−x)  */
    seq_t answer = 0;
    for (idx_t x = 1; x <= n - 2; ++x) {
        seq_t prod = sigma[x] * pair_best[n - x];
        if (prod > answer) answer = prod;
    }

    mem_free(sigma);
    mem_free(pair_best);
    return answer;
}

/* external entry point required by the build system */
seq_t a(idx_t n)
{
    if (n < 3) return 0;               /* undefined below offset 3 */
    return maximum_product(n);
}
