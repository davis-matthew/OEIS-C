/*
 * A033752  Expansion of Product_{d | 40} theta_3(q^d).
 * https://oeis.org/A033752
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = coefficient of q^n in ∏_{d|40} θ₃(q^d), equal to the
 *            number of representations of n as n = 1·k₁² + 2·k₂² + 4·k₃² +
 *            5·k₄² + 8·k₅² +10·k₆² +20·k₇² +40·k₈² with each non‑zero k_i
 *            contributing a factor 2
 * Range:     u8 n <= 27, u16 n <= 179, u32 n <= 7094, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdbool.h>

/* Static cache for previously computed terms. */
static seq_t *cache   = NULL;
static idx_t  cache_sz = 0;   /* valid indices 0 .. cache_sz-1 */

/* Recompute the cache up to include index new_n (inclusive). */
static void recompute_up_to(idx_t new_n)
{
    /* Allocate a working array for the convolution result. */
    seq_t *cur = (seq_t *)calloc(new_n + 1, sizeof(seq_t));
    if (!cur) abort();           /* allocation failure */
    cur[0] = (seq_t)1;           /* start with coefficient 1 at q^0 */

    /* Divisors of 40 in ascending order. */
    const idx_t divisors[8] = {1, 2, 4, 5, 8, 10, 20, 40};

    for (size_t di = 0; di < 8; ++di) {
        idx_t d = divisors[di];

        /* Allocate next array for convolution with θ₃(q^d). */
        seq_t *next = (seq_t *)calloc(new_n + 1, sizeof(seq_t));
        if (!next) abort();

        /* Perform the convolution. */
        for (idx_t j = 0; j <= new_n; ++j) {
            seq_t val = cur[j];
            if (val == 0) continue;

            /* Contribution from k = 0 (coefficient 1). */
            next[j] += val;

            /* Contributions from non‑zero k. */
            for (idx_t m = 1; ; ++m) {
                idx_t m2 = m * m;
                idx_t p  = d * m2;               /* d·m²   */
                if (p > new_n) break;
                idx_t idx = j + p;
                if (idx > new_n) break;
                next[idx] += val * ((seq_t)2);   /* 2 * val */
            }
        }

        /* Prepare for next divisor. */
        free(cur);
        cur = next;
    }

    /* Replace old cache with the newly computed values. */
    free(cache);
    cache   = cur;
    cache_sz = new_n + 1;   /* indices 0 .. new_n are valid now */
}

/* Public interface required by the framework. */
seq_t a(idx_t n)
{
    if (n >= cache_sz)
        recompute_up_to(n);
    return cache[n];
}
