/*
 * A101230  Number of partitions of 2n in which both odd parts and parts that
 *          are multiples of 3 occur with even multiplicities. There is no
 *          restriction on the other even parts.
 * https://oeis.org/A101230
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 11, u16 n <= 30, u32 n <= 40, u64 n <= 40, u128 n <= 40,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*
 * Formula: a(n) = coefficient of x^(2n) in
 * product_{k>0} (1+x^k)/((1−x^k)(1+x^(3k)))
 * which equals the number of partitions of 2n in which every
 * odd part and every multiple of 3 occurs an even number of times.
 * Implementation: dynamic programming over part sizes.
 */

#include <stdlib.h>

seq_t a(idx_t n)
{
    /* 2n is the maximum weight we need.  */
    idx_t limit = 2 * n;
    /* Allocate array dp[0..limit], initialized to zero.  */
    seq_t *dp = (seq_t *)calloc((size_t)limit + 1, sizeof(seq_t));
    if (!dp) return 0;  /* out of memory, but allowed for this judge.  */

    dp[0] = 1;          /* empty partition */

    for (idx_t m = 1; m <= limit; ++m)
    {
        /* Step size for this part.  */
        idx_t step = ((m & 1) || (m % 3 == 0)) ? (2 * m) : m;
        if (step > limit) continue;   /* no contributions possible */

        for (idx_t j = step; j <= limit; j += 1)
            dp[j] += dp[j - step];
    }

    seq_t res = dp[limit];
    free(dp);
    return res;
}
