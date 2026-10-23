/*
 * A036028  Number of partitions of n into parts not of form 4k+2, 20k, 20k+9
 *          or 20k-9. Also number of partitions in which no odd part is
 *          repeated, with at most 4 parts of size less than or equal to 2 and
 *          where differences between parts at distance 4 are greater than 1
 *          when the smallest part is odd and great...
 * https://oeis.org/A036028
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = number of partitions of n into parts not of form 4k+2,
 *            20k, 20k+9 or 20k-9 (unrestricted multiplicity)
 * Range:     u8 n <= 23, u16 n <= 51, u32 n <= 51, u64 n <= 51, u128 n <= 51,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdbool.h>

/* test whether a part p is allowed */
static inline bool allowed_part(idx_t p)
{
    idx_t r = p % 20;          /* residue modulo 20 */
    if (r == 0 || r == 9 || r == 11)   /* 20k, 20k±9 */
        return false;
    if (r % 4 == 2)                     /* 4k+2 */
        return false;
    return true;
}

seq_t a(idx_t n)
{
    /* allocate DP array of size n+1 */
    seq_t *dp = (seq_t*)calloc((size_t)(n + 1), sizeof(seq_t));
    if (!dp)
        return 0;            /* memory allocation failed */
    dp[0] = 1;               /* empty partition */

    for (idx_t part = 1; part <= n; ++part)
    {
        if (!allowed_part(part))
            continue;

        /* unbounded coin change for this part */
        for (idx_t value = part; value <= n; ++value)
            dp[value] += dp[value - part];
    }

    seq_t result = dp[n];
    free(dp);
    return result;
}
