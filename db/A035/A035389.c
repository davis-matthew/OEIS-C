/*
 * A035389  Number of partitions of n into parts 6k+4 or 6k+5.
 * https://oeis.org/A035389
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = number of partitions of n whose parts are ≡4 or 5 (mod 6)
 * Range:     u8 n <= 64, u16 n <= 71, u32 n <= 71, u64 n <= 71, u128 n <= 71,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

/* Static state to memoize computed values up to the largest required n. */
static seq_t *dp      = NULL;   /* array of partition counts            */
static idx_t  max_n   = 0;      /* largest n for which dp is valid      */

/* Recompute the partition table up to new_max (inclusive). */
static void recompute(idx_t new_max) {
    seq_t *new_dp = (seq_t *)malloc((new_max + 1) * sizeof(seq_t));
    /* Initialise all entries to 0. */
    for (idx_t i = 0; i <= new_max; ++i) new_dp[i] = 0;
    new_dp[0] = 1;   /* empty partition */

    /* Process parts congruent to 4 modulo 6. */
    for (idx_t k = 0; (4 + 6 * k) <= new_max; ++k) {
        idx_t part = 4 + 6 * k;
        for (idx_t i = part; i <= new_max; ++i)
            new_dp[i] += new_dp[i - part];
    }

    /* Process parts congruent to 5 modulo 6. */
    for (idx_t k = 0; (5 + 6 * k) <= new_max; ++k) {
        idx_t part = 5 + 6 * k;
        for (idx_t i = part; i <= new_max; ++i)
            new_dp[i] += new_dp[i - part];
    }

    /* Replace old table. */
    free(dp);
    dp      = new_dp;
    max_n   = new_max;
}

/* Public function computing a(n). */
seq_t a(idx_t n) {
    if (n == 0) return 0;       /* offset starts at 1, but harmless */
    if (n > max_n) recompute(n);
    return dp[n];
}
