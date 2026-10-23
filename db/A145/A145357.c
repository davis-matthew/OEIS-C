/*
 * A145357  Lower triangular array, called S1hat(6), related to partition
 *          number array A145356.
 * https://oeis.org/A145357
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy, tabl
 * Formula:   a(n) is the entry of the lower triangular array S1hat(6)
 *            obtained by flattening its rows. Each entry S1hat(6)(s,k) is the
 *            sum over all partitions of s into exactly k parts of the product
 *            of weights w(j) = (j+4)!/120 for each part j
 * Range:     u8 n <= 1, u16 n <= 10, u32 n <= 36, u64 n <= 50, u128 n <= 50,
 *            big any n
 * Helpers:   seq_from_str
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <math.h>
#include <stdlib.h>
#include <stdint.h>
#include "../utils/seq_from_str.h"

static seq_t *dp = NULL;          /* dp[s * (maxR+1) + k] = S1hat(6)(s,k) */
static seq_t *w  = NULL;          /* weights w[j] for part size j  */
static seq_t *fact = NULL;        /* factorials fact[i] = i!  */
static idx_t  maxR = 0;           /* largest row for which dp is built */

/* compute floor((sqrt(8*n+1)-1)/2) safely with 128‑bit integers   */
static inline idx_t row_of_index(idx_t n)
{
    idx_t r = (idx_t)((sqrt(8.0 * (double)n + 1.0) - 1.0) / 2.0);
    /* adjust up or down if needed */
    while ( ( (__uint128_t)r * (r + 1) ) / 2 < n ) r++;
    while ( r > 0 && ( (__uint128_t)(r - 1) * r ) / 2 >= n ) r--;
    return r;
}

/* rebuild dp, w and fact arrays for all rows up to newR (inclusive) */
static void rebuild(idx_t newR)
{
    /* free old arrays if any */
    if (dp) free(dp);
    if (w)  free(w);
    if (fact) free(fact);

    /* allocate new arrays */
    dp   = (seq_t*)calloc((newR + 1) * (newR + 1), sizeof(seq_t));
    w    = (seq_t*)calloc(newR + 1, sizeof(seq_t));
    fact = (seq_t*)calloc(newR + 5, sizeof(seq_t));  /* need up to newR+4 */

    /* factorials */
    fact[0] = seq_from_str("1");
    for (idx_t i = 1; i <= newR + 4; ++i)
        fact[i] = fact[i - 1] * (seq_t)i;

    /* weights w[j] = fact[j+4] / 120 */
    const seq_t div120 = (seq_t)120;
    for (idx_t j = 1; j <= newR; ++j)
        w[j] = fact[j + 4] / div120;

    /* dp[0][0] = 1 */
    dp[0] = seq_from_str("1");

    /* dynamic programming for partitions */
    for (idx_t j = 1; j <= newR; ++j)
    {
        seq_t weight = w[j];
        for (idx_t s = j; s <= newR; ++s)
        {
            idx_t baseIdx = s * (newR + 1);
            idx_t prevIdx = (s - j) * (newR + 1);
            for (idx_t k = 1; k <= s; ++k)
            {
                seq_t term = dp[prevIdx + k - 1] * weight;
                dp[baseIdx + k] += term;
            }
        }
    }

    maxR = newR;
}

seq_t a(idx_t n)
{
    if (n == 0) return seq_from_str("0");  /* offset is 1, but guard */

    /* determine the row (r) and column (c) in the flattened triangle */
    idx_t r = row_of_index(n);
    idx_t c = n - ( (r - 1) * r ) / 2;   /* 0‑based column offset */

    /* ensure DP tables are large enough */
    if (r > maxR)
        rebuild(r);

    return dp[r * (maxR + 1) + c];
}
