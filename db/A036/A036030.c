/*
 * A036030  Number of partitions of n into parts not of form 4k+2, 24k, 24k+3
 *          or 24k-3. Also number of partitions in which no odd part is
 *          repeated, with 1 part of size less than or equal to 2 and where
 *          differences between parts at distance 5 are greater than 1 when
 *          the smallest part is odd and greater than 2...
 * https://oeis.org/A036030
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) counts partitions of n using parts that are NOT congruent
 *            to * 2 mod 4, 0 mod 24, 3 mod 24 or 21 mod 24 (i.e., 24k‑3).
 *            This is * implemented by a standard coin‑change dynamic
 *            programming recurrence
 * Range:     u8 n <= 27, u16 n <= 53, u32 n <= 53, u64 n <= 53, u128 n <= 53,
 *            big any n
 * Helpers:   alloc_seq_array, free_seq_array
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/alloc_seq_array.h"
#include "../utils/free_seq_array.h"

seq_t a(idx_t n)
{
    if (n == 0) return 0;            /* a(0) is unused but guarded */
    seq_t *dp = alloc_seq_array(n + 1);  /* dp[i] = number of admissible partitions of i */
    dp[0] = 1;                        /* empty partition of 0 */

    for (idx_t part = 1; part <= n; ++part) {
        if ((part % 4 == 2) ||
            (part % 24 == 0) ||
            (part % 24 == 3) ||
            (part % 24 == 21))
            continue;                 /* skip forbidden part sizes */

        for (idx_t sum = part; sum <= n; ++sum)
            dp[sum] += dp[sum - part];
    }

    seq_t result = dp[n];
    free_seq_array(dp);
    return result;
}
