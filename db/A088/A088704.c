/*
 * A088704  a(n) equals the coefficient of x^n in f(x)^n where
 *          f(x)=1+sum(n>=0,x^(2^n)).
 * https://oeis.org/A088704
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = coefficient of x^n in (1 + sum_{k>=0} x^{2^k})^n
 * Range:     u8 n <= 6, u16 n <= 10, u32 n <= 19, u64 n <= 25, u128 n <= 25,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>

/* Return the nth term of sequence A088704.  The function
   may be called with arbitrary indices, but we compute it
   directly from scratch for each call.  Complexity O(n^2).  */
seq_t a(idx_t n)
{
    /* Special case n = 0 : 1 */
    if (n == 0)
        return (seq_t)1;

    /* Allocate DP arrays, size n+1 */
    size_t len = (size_t)n + 1;
    seq_t *prev = (seq_t *)calloc(len, sizeof(seq_t));
    seq_t *curr = (seq_t *)calloc(len, sizeof(seq_t));
    if (!prev || !curr) {
        /* In case of allocation failure, return 0 (unlikely) */
        return (seq_t)0;
    }

    /* Base case for 0 positions: only sum 0 possible */
    prev[0] = (seq_t)1;

    /* Precompute powers of two up to n */
    size_t pow_cnt = 0;
    size_t pow_buf[16];          /* n <= 10000 so at most 14 powers */
    for (size_t p = 1; p <= (size_t)n; p <<= 1) {
        pow_buf[pow_cnt++] = p;
    }

    /* DP over positions */
    for (size_t pos = 1; pos <= (size_t)n; ++pos) {
        /* Zero part contribution: copy previous row */
        for (size_t s = 0; s <= (size_t)n; ++s) {
            curr[s] = prev[s];
        }
        /* Add contributions of each power of two */
        for (size_t pi = 0; pi < pow_cnt; ++pi) {
            size_t p = pow_buf[pi];
            for (size_t s = p; s <= (size_t)n; ++s) {
                curr[s] += prev[s - p];
            }
        }
        /* Prepare for next position */
        seq_t *tmp = prev;
        prev = curr;
        curr = tmp;
    }

    /* Result: number of ways to fill n positions with sum n */
    seq_t result = prev[(size_t)n];

    free(prev);
    free(curr);
    return result;
}
