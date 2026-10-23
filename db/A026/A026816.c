/*
 * A026816  Number of partitions of n in which the greatest part is 10.
 * https://oeis.org/A026816
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = number of partitions of n in which the greatest part
 *            equals 10. This equals the number of partitions of n-10 with
 *            parts ≤10, i.e. the coefficient of x^n in x^10 / Π_{k=1}^{10}
 *            (1-x^k)
 * Range:     u8 n <= 26, u16 n <= 60, u32 n <= 244, u64 n <= 3053,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

/* dynamic programming cache for partitions with parts ≤10 */
static seq_t *dp = NULL;   /* dp[i] = #partitions of i with parts ≤10 */
static idx_t  dp_size = 0; /* maximum index that dp currently covers */

/* recompute dp[0..new_size] from scratch */
static void recompute_dp(idx_t new_size) {
    /* resize array */
    seq_t *tmp = (seq_t *)realloc(dp, (new_size + 1) * sizeof(seq_t));
    if (!tmp) { /* allocation failed – leave dp as NULL to avoid reuse */
        dp = NULL;
        dp_size = 0;
        return;
    }
    dp = tmp;
    dp_size = new_size;

    /* initialise */
    for (idx_t i = 0; i <= dp_size; ++i) dp[i] = 0;
    dp[0] = 1;                       /* empty partition */

    /* dynamic programming: for each part k = 1..10, add its contributions */
    for (idx_t part = 1; part <= 10; ++part) {
        for (idx_t s = part; s <= dp_size; ++s) {
            dp[s] = dp[s] + dp[s - part];
        }
    }
}

seq_t a(idx_t n) {
    if (n < 10) return 0;              /* cannot contain a part of size 10 */

    idx_t target = n - 10;              /* need partitions of target with parts ≤10 */

    /* initialise or grow the cache as needed */
    if (dp == NULL) {
        recompute_dp(target);
    } else if (target > dp_size) {
        recompute_dp(target);
    }

    return dp[target];
}
