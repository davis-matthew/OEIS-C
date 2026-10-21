/*
 * A219485  Number of representations of n as a sum of distinct elements of
 *          the generalized Fibonacci sequence beginning 3, 7, 10, 17, 27, 44,
 *          ....
 * https://oeis.org/A219485
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 635, u16 n <= 9325, u32 n <= 9325, u64 n <= 9325,
 *            u128 n <= 9325, big any n
 * Helpers:   generate_fib_like
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Number of representations of n as a sum of distinct elements of the
   generalized Fibonacci sequence 3,7,10,17,27,44,….
   Computes the number of subsets of that sequence whose sum equals n
   using a standard subset‑sum dynamic programming algorithm. */
#include <stdlib.h>
#include <stdbool.h>
#include "../utils/generate_fib_like.h"

seq_t a(idx_t n)
{
    /* Convert the index to a seq_t for uniform handling in comparisons. */
    seq_t seq_n = (seq_t)n;

    /* Generate the generalized Fibonacci numbers up to n. */
    /* 25 is ample for n ≤ 10000 (about 15 terms). */
    seq_t fib[25];
    size_t fibcnt = generate_fib_like(seq_n, fib, sizeof fib / sizeof fib[0]);

    /* Allocate dynamic programming array with zero initialization. */
    seq_t *ways = (seq_t *)calloc((size_t)n + 1, sizeof(seq_t));
    if (!ways) return (seq_t)0;            /* allocation failure → 0 */
    ways[0] = (seq_t)1;                    /* empty subset sums to 0 */

    /* For each Fibonacci number, update the DP array in reverse order
       to enforce distinctness of elements. */
    for (size_t idx = 0; idx < fibcnt; ++idx) {
        idx_t val = (idx_t)fib[idx];
        if (val > n) continue;            /* should not happen, guard nonetheless */
        for (idx_t sum = n; sum >= val; --sum) {
            ways[sum] += ways[sum - val];
        }
    }

    seq_t result = ways[(size_t)n];
    free(ways);
    return result;
}
