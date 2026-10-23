/*
 * A035640  Number of partitions of n into parts 6k and 6k+4 with at least one
 *          part of each type.
 * https://oeis.org/A035640
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 61, u16 n <= 145, u32 n <= 411, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   alloc_seq_array, free_seq_array
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A035640:
 * Number of partitions of n into parts 6k and 6k+4 with at least one part of each type
 * (offset 1).  The generating function is (P₄(x)-1)(P₆(x)-1) where
 * P₄(x)=1/∏_{k≥0}(1-x^(6k+4)) and P₆(x)=1/∏_{k≥1}(1-x^(6k)).
 * We compute this by dynamic programming.
 */

#include <stdlib.h>
#include "../utils/alloc_seq_array.h"
#include "../utils/free_seq_array.h"

/* Count partitions of n using only parts of the form start+step*m
 * (start >= step).  The array 'dp' is allocated with alloc_seq_array
 * and must be freed by free_seq_array by the caller. */
static seq_t parts_only(idx_t n, idx_t start, idx_t step)
{
    if (n == 0) return (seq_t)1;  /* unique partition of 0 */

    seq_t *dp = alloc_seq_array(n + 1);
    if (!dp) return (seq_t)0;      /* allocation failure → 0 */

    dp[0] = (seq_t)1;
    for (idx_t i = 1; i <= n; ++i) dp[i] = (seq_t)0;

    for (idx_t v = start; v <= n; v += step) {
        for (idx_t i = v; i <= n; ++i) {
            dp[i] += dp[i - v];
        }
    }

    seq_t result = dp[n];
    free_seq_array(dp);
    return result;
}

/* Count partitions of n using parts of both types (4 mod 6 and multiples of 6).
 * We run the DP over all allowed parts in any order; the result is independent
 * of the order. */
static seq_t parts_union(idx_t n)
{
    if (n == 0) return (seq_t)1;

    seq_t *dp = alloc_seq_array(n + 1);
    if (!dp) return (seq_t)0;

    dp[0] = (seq_t)1;
    for (idx_t i = 1; i <= n; ++i) dp[i] = (seq_t)0;

    /* Add parts congruent to 4 mod 6 */
    for (idx_t v = 4; v <= n; v += 6) {
        for (idx_t i = v; i <= n; ++i) {
            dp[i] += dp[i - v];
        }
    }

    /* Add parts that are multiples of 6 */
    for (idx_t v = 6; v <= n; v += 6) {
        for (idx_t i = v; i <= n; ++i) {
            dp[i] += dp[i - v];
        }
    }

    seq_t result = dp[n];
    free_seq_array(dp);
    return result;
}

seq_t a(idx_t n)
{
    /* All partitions using the two types */
    seq_t total = parts_union(n);
    /* Partitions using only multiples of 6 or only 4 mod 6 */
    seq_t only6 = parts_only(n, 6, 6);
    seq_t only4 = parts_only(n, 4, 6);
    /* Subtract the partitions that use only one type.
     * The empty partition (n=0) is irrelevant for n≥1. */
    seq_t res = total;
    res -= only6;
    res -= only4;
    return res;
}
