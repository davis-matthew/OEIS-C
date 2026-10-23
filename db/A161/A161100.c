/*
 * A161100  Number of partitions of n into squares where every part appears at
 *          least 12 times.
 * https://oeis.org/A161100
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 271, u16 n <= 826, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Generate the OEIS sequence A161100: Number of partitions of n
   into squares where every part appears at least 12 times. */

#include <stddef.h>
#include <stdlib.h>
#include <string.h>

seq_t a(idx_t n)
{
    /* No partition can satisfy the minimum 12 copies unless n >= 12 */
    if (n < 12)
        return 0;

    /* dp[w] will hold the number of partitions of weight w considered so far */
    idx_t size = n + 1;
    seq_t *dp = (seq_t *)calloc((size_t)size, sizeof(seq_t));
    seq_t *tmp = (seq_t *)calloc((size_t)size, sizeof(seq_t));
    if (!dp || !tmp) {               /* allocation failed */
        free(dp);
        free(tmp);
        return 0;
    }
    dp[0] = (seq_t)1;                 /* empty partition */

    for (idx_t k = 1; ; ++k) {
        idx_t v = k * k;              /* square size */
        if (v > n)
            break;

        idx_t start = (idx_t)(v * 12);   /* weight of the mandatory 12 copies */
        if (start > n)                /* this square cannot be used */
            continue;

        /* tmp holds contributions involving this square */
        memset(tmp, 0, (size_t)size * sizeof(seq_t));

        /* exactly 12 copies of v */
        for (idx_t w = start; w <= n; ++w)
            tmp[w] = dp[w - start];

        /* add unlimited extra copies of v beyond the mandatory 12 */
        for (idx_t w = start + v; w <= n; ++w)
            tmp[w] += tmp[w - v];

        /* merge contributions into dp */
        for (idx_t w = 0; w <= n; ++w)
            dp[w] += tmp[w];
    }

    seq_t result = dp[n];
    free(dp);
    free(tmp);
    return result;
}
