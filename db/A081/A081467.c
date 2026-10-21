/*
 * A081467  a(n) = smallest (n+k) such that n divides the sum (n+1)+ (n+2) +
 *          ... (n+k) = k*n + k*(k+1)/2.
 * https://oeis.org/A081467
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = n + min{k ≥ 1 | n divides k(k+1)/2}
 * Range:     u8 n <= 16, u16 n <= 73, u32 n <= 73, u64 n <= 73, u128 n <= 73,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
/* The types seq_t and idx_t are provided by the build system.  */
seq_t a(idx_t n)
{
    /* n is at least 1.  Search for the smallest k such that
       n | k*(k+1)/2.  Empirically k never exceeds 2*n, and
       the search is fast enough for n up to at least 10 000.   */
    for (idx_t k = 1; k <= 2 * n; ++k)
    {
        seq_t prod = (seq_t)k * (k + 1);
        prod /= 2;               /* now prod = k(k+1)/2 */
        if (prod % n == 0)
        {
            return (seq_t)n + k;
        }
    }
    /* In theory the above loop always finds a solution.  If not,
       return the trivial upper bound 3*n as a safeguard.  */
    return (seq_t)n + 3 * n;
}
