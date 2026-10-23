/*
 * A035433  Number of partitions of n into parts 7k+2 or 7k+5.
 * https://oeis.org/A035433
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 59, u16 n <= 149, u32 n <= 436, u64 n <= 1422,
 *            u128 n <= 5006, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* OEIS A035433: Number of partitions of n into parts 7k+2 or 7k+5.
   The sequence is computed by the classical coin‑change dynamic
   programming algorithm. For each admissible part v (v ≡ 2 or 5 (mod 7)),
   we add its contribution to all larger indices.  The routine
   recomputes the entire table up to the requested n on each call.
   This guarantees correctness for any n ≥ 0 and satisfies all
   integer‑only requirements. */
#include <stdlib.h>

seq_t a(idx_t n)
{
    /* allocate table for 0 … n */
    seq_t *dp = (seq_t *)calloc((size_t)(n + 1), sizeof(seq_t));
    if (!dp)
        return (seq_t)0;                 /* allocation failed */

    dp[0] = (seq_t)1;                    /* empty partition */

    for (idx_t v = 2; v <= n; ++v)
    {
        if ((v % 7 == 2) || (v % 7 == 5))
        {
            for (idx_t i = v; i <= n; ++i)
            {
                dp[i] += dp[i - v];
            }
        }
    }

    seq_t result = dp[n];
    free(dp);
    return result;
}
