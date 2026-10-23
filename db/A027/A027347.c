/*
 * A027347  Number of partitions of n into distinct odd parts, the least being
 *          congruent to 1 mod 4.
 * https://oeis.org/A027347
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   number of partitions of n into distinct odd parts with the least
 *            part ≡ 1 (mod 4)
 * Range:     u8 n <= 68, u16 n <= 68, u32 n <= 68, u64 n <= 68, u128 n <= 68,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

static seq_t *dp1 = NULL;   /* partitions with smallest part ≡ 1 (mod 4) */
static seq_t *dp3 = NULL;   /* partitions with smallest part ≡ 3 (mod 4) */
static idx_t max_computed = 0;

/* recompute DP tables up to 'max' */
static void recompute(idx_t max) {
    /* allocate or resize arrays */
    dp1 = (seq_t *)realloc(dp1, (max + 1) * sizeof(seq_t));
    dp3 = (seq_t *)realloc(dp3, (max + 1) * sizeof(seq_t));
    if (!dp1 || !dp3) {
        abort(); /* out of memory */
    }
    /* initialise to zero */
    for (idx_t i = 0; i <= max; ++i) {
        dp1[i] = 0;
        dp3[i] = 0;
    }

    /* DP for distinct odd parts */
    for (idx_t k = 1; k <= max; k += 2) {
        for (idx_t n = max; n >= k; --n) {
            dp1[n] += dp1[n - k];
            dp3[n] += dp3[n - k];
        }
        /* add the subset consisting of only k */
        if ((k & 3) == 1) {
            dp1[k] += (seq_t)1;
        } else {
            dp3[k] += (seq_t)1;
        }
    }
    max_computed = max;
}

/* ensure tables are ready up to n */
static void ensure_dp(idx_t n) {
    if (n > max_computed) {
        recompute(n);
    }
}

seq_t a(idx_t n) {
    ensure_dp(n);
    return dp1[n];
}
