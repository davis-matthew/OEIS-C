/*
 * A099262  a(n) = (1/5040)*7^n + (1/240)*5^n + (1/72)*4^n + (1/16)*3^n +
 *          (11/60)*2^n + 53/144. Partial sum of Stirling numbers of second
 *          kind S(n,i), i=1..7 (i.e., a(n) = Sum_{i=1..7} S(n,i)).
 * https://oeis.org/A099262
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = Sum_{k=0..7} S(n,k) where S(n,k) are Stirling numbers of
 *            the second kind (partial sums over k=1..7 plus the S(n,0)=0 for
 *            n>0, S(0,0)=1)
 * Range:     u8 n <= 6, u16 n <= 9, u32 n <= 15, u64 n <= 26, u128 n <= 26,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

seq_t a(idx_t n)
{
    /* DP arrays for S values up to k=7. */
    seq_t prev[8] = {0};
    seq_t cur[8] = {0};

    /* Initial row for n = 0: S(0,0)=1. */
    prev[0] = 1;

    /* Iterate from 1 to n computing S(i, k) row by row. */
    for (idx_t i = 1; i <= n; ++i) {
        cur[0] = 0;                /* S(i,0) = 0 for i>0 */
        for (int k = 1; k <= 7; ++k) {
            /* S(i,k) = k*S(i-1,k) + S(i-1,k-1) */
            cur[k] = (seq_t)k * prev[k] + prev[k - 1];
        }
        /* Roll arrays: cur becomes the new prev for next iteration. */
        for (int k = 0; k <= 7; ++k) {
            prev[k] = cur[k];
        }
    }

    /* Sum of S(n,k) for k = 0..7 gives the required sequence term. */
    seq_t result = 0;
    for (int k = 0; k <= 7; ++k) {
        result += prev[k];
    }
    return result;
}
