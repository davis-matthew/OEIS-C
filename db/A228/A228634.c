/*
 * A228634  Number of s in {1,...,n}^n having shortest run with the same value
 *          of length 8.
 * https://oeis.org/A228634
 *
 * Offset:    8    (a(8) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = n * Σ_{k=1..⌊n/8⌋} (n-1)^{k-1} * * ( C(n-7k-1, k-1) -
 *            C(n-8k-1, k-1) ) * (runs of equal symbols have length ≥8, at
 *            least one run has length exactly 8)
 * Range:     u8 n <= 16, u16 n <= 25, u32 n <= 42, u64 n <= 73, u128 n <= 130,
 *            big any n
 * Helpers:   binom
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/binom.h"   // provides binom(idx_t a, idx_t b) returning seq_t

seq_t a(idx_t n)
{
    if (n < 8) return 0;

    seq_t result = 0;          // running total of compositions * (n-1)^{k-1}
    seq_t power   = 1;         // (n-1)^{k-1} for current k, start with k=1

    seq_t nm1 = (seq_t)n - 1;  // base for exponentiation, cast once

    idx_t kmax = n / 8;        // maximum number of runs

    for (idx_t k = 1; k <= kmax; ++k)
    {
        /* components of the composition count */
        seq_t first  = 0;   // C(n-7k-1, k-1)  (exists iff n >= 7k+1)
        seq_t second = 0;   // C(n-8k-1, k-1)  (exists iff n >= 8k+1)

        if (n >= 7*k + 1)            /* n-7k-1 >= 0 */
            first = binom(n - 7*k - 1, k - 1);

        if (n >= 8*k + 1)            /* n-8k-1 >= 0 */
            second = binom(n - 8*k - 1, k - 1);

        /* difference counts compositions with at least one part = 8 */
        seq_t diff = first;
        if (second != 0)
            diff -= second;       /* first >= second always holds combinatorially */

        /* accumulate term: power * diff */
        if (diff != 0)
            result += power * diff;

        /* update power for next k */
        power *= nm1;              // (n-1)^{k}
    }

    result *= (seq_t)n;            // multiply by n for choice of first value
    return result;
}
