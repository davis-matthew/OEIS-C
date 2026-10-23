/*
 * A104502  Number of partitions where no part is a multiple of 9.
 * https://oeis.org/A104502
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = number of partitions of n into parts none divisible by 9.
 *            (⇑ a(0)=1, a(1)=1, a(2)=2, a(3)=3, a(4)=5, … from OEIS A104502)
 * Range:     u8 n <= 16, u16 n <= 44, u32 n <= 135, u64 n <= 448,
 *            u128 n <= 1592, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

static seq_t *dp = NULL;      /* memoised DP table */
static idx_t dp_size = 0;     /* current allocated size (dp_size-1 is largest index computed) */

/* Allocate dp array and recompute it completely up to new_n (inclusive). */
static void extend_to(idx_t new_n)
{
    if (new_n + 1 <= dp_size)            /* already covered */
        return;

    /* reallocate memory; cast is needed for C when seq_t is a class */
    seq_t *new_dp = (seq_t *)realloc(dp, (size_t)(new_n + 1) * sizeof(seq_t));
    if (!new_dp)
        abort();                         /* allocation failed */
    dp = new_dp;

    /* initialise all entries to zero, then set dp[0] = 1 */
    seq_t zero = 0;
    for (idx_t i = 0; i <= new_n; ++i)
        dp[i] = zero;
    dp[0] = 1;

    /* dynamic‑programming: accumulate partitions, skipping parts that are multiples of 9 */
    for (idx_t part = 1; part <= new_n; ++part) {
        if (part % 9 == 0)
            continue;
        for (idx_t sum = part; sum <= new_n; ++sum)
            dp[sum] += dp[sum - part];
    }

    dp_size = new_n + 1;
}

seq_t a(idx_t n)
{
    extend_to(n);
    return dp[n];
}
