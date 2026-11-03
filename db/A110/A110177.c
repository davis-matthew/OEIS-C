/*
 * A110177  Number of solutions 0<k<n to the equation sigma(n) = sigma(k) +
 *          sigma(n-k), where sigma is the sum of divisors function.
 * https://oeis.org/A110177
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = #{k | 0 < k < n, σ(n) = σ(k) + σ(n-k)}
 * Range:     u8 n <= 1515, u16 n <= 16384, u32 n <= 16384, u64 n <= 16384,
 *            u128 n <= 16384, big any n
 * Validated: standard, pass, 121 terms through n = 9340 (2026-11-03)
 * Helpers:   mem
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* NOTE: No external utilities are used.  sigma is cached across calls. */

#include <stdlib.h>   /* malloc, free */
#include "../utils/mem.h"

/* static cache of divisor-sum values */
static seq_t *sigma = NULL;
static idx_t sigma_max = 0;

/* recompute σ[1..n] from scratch */
static void recompute_sigma(idx_t n)
{
    if (sigma != NULL) {
        mem_free(sigma);
    }
    /* allocate array 0..n, but sigma[0] is unused */
    sigma = mem_new(seq_t, (n + 1));
    for (idx_t i = 0; i <= n; ++i) {
        sigma[i] = 0;
    }

    for (idx_t i = 1; i <= n; ++i) {
        seq_t vi = (seq_t)i;
        for (idx_t j = i; j <= n; j += i) {
            sigma[j] += vi;
        }
    }
    sigma_max = n;
}

/* OEIS function */
seq_t a(idx_t n)
{
    if (n == 0) {
        return 0;  /* not used, offset starts at 1 */
    }

    if (n > sigma_max) {
        recompute_sigma(n);
    }
    /* if n <= sigma_max, sigma already contains needed values */

    seq_t target = sigma[n];
    seq_t cnt = 0;
    for (idx_t k = 1; k < n; ++k) {
        if (sigma[k] + sigma[n - k] == target) {
            ++cnt;
        }
    }
    return cnt;
}
