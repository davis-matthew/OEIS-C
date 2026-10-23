/*
 * A158166  Number of nondecreasing integer sequences of length 32 with sum
 *          zero and sum of absolute values 2n.
 * https://oeis.org/A158166
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = sum_{a=1}^{32} sum_{b=1}^{32-a} p(n,a) * p(n,b) where
 *            p(n,k) = number of integer partitions of n into exactly k parts
 * Range:     u8 n <= 7, u16 n <= 16, u32 n <= 43, u64 n <= 147, u128 n <= 235,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stddef.h>
#include <stdlib.h>
#include <string.h>

#define PARTS 32
#define K1 (PARTS + 1)

/* Static DP table: dp[n*K1 + k] = p(n,k) for 0 <= k <= PARTS. */
static seq_t *dp = NULL;
static idx_t max_n_comp = 0;

/* Grow the DP table to compute p(n,k) for all n <= nmax. */
static void ensure_dp(idx_t nmax) {
    if (nmax <= max_n_comp)
        return;

    size_t old_rows = (size_t)(max_n_comp + 1);
    size_t new_rows = (size_t)(nmax + 1);
    size_t old_size = old_rows * K1;
    size_t new_size = new_rows * K1;

    /* Allocate (or reallocate) the table. */
    seq_t *newdp = (seq_t *)realloc(dp, new_size * sizeof(seq_t));
    if (!newdp) {
        /* Allocation failed: leave dp unchanged and let caller handle. */
        return;
    }
    dp = newdp;

    /* Zero the new memory region.  The memory is zero-initialised. */
    if (new_size > old_size) {
        void *ptr = dp + old_size;
        memset(ptr, 0, (new_size - old_size) * sizeof(seq_t));
    }

    /* Base row n = 0: p(0,0) = 1, other entries already zero. */
    dp[0 * K1 + 0] = 1;

    /* Compute remaining rows. */
    for (idx_t n = max_n_comp + 1; n <= nmax; ++n) {
        size_t base = (size_t)n * K1;
        dp[base + 0] = 0;  /* p(n,0) = 0 for n > 0 */
        for (idx_t k = 1; k <= PARTS; ++k) {
            seq_t val = dp[(size_t)(n - 1) * K1 + (k - 1)];
            if (n >= k) {
                val += dp[(size_t)(n - k) * K1 + k];
            }
            dp[base + k] = val;
        }
    }

    max_n_comp = nmax;
}

/* Main sequence function. */
seq_t a(idx_t n) {
    if (n == 0) return 0;  /* undefined for n = 0 in OEIS context */

    ensure_dp(n);

    seq_t result = 0;
    /* a and b must be at least 1.  They also cannot exceed PARTS and n. */
    idx_t max_a = (n < PARTS) ? n : PARTS;
    for (idx_t a = 1; a <= max_a; ++a) {
        seq_t pa = dp[(size_t)n * K1 + a];
        if (pa == 0) continue;
        idx_t max_b = ((a + 1) <= PARTS) ? PARTS - a : 0;
        /* b must be at least 1. */
        for (idx_t b = 1; b <= max_b; ++b) {
            seq_t pb = dp[(size_t)n * K1 + b];
            if (pb == 0) continue;
            result += pa * pb;
        }
    }

    return result;
}
