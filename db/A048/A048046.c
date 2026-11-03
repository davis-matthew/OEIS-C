/*
 * A048046  Number of nonempty subsets of {1,2,...,n} in which exactly 4/5 of
 *          the elements are <= (n+1)/3.
 * https://oeis.org/A048046
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Σ_{m≥1, 4 m ≤ ⌊(n+1)/3⌋} binom(⌊(n+1)/3⌋, 4 m) *
 *            binom(n−⌊(n+1)/3⌋, m)
 * Range:     u8 n <= 19, u16 n <= 34, u32 n <= 61, u64 n <= 118,
 *            u128 n <= 229, big any n
 * Validated: standard, pass, 59 terms through n = 5653 (2026-11-01)
 * Helpers:   binomial
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Counting nonempty subsets of {1,…,n} in which exactly 4/5 of the elements are ≤ floor((n+1)/3). */

#include <stdint.h>
#include "../utils/binomial.h"

seq_t a(idx_t n)
{
    /* Compute the threshold t = floor((n+1)/3) and the larger part L = n – t */
    idx_t t = (n + 1) / 3;
    idx_t L = n - t;

    seq_t total = 0;

    for (idx_t m = 1; 4 * m <= t && m <= L; ++m)
    {
        seq_t small = binomial(t, 4 * m);   /* choose 4m elements from the first t */
        seq_t large = binomial(L, m);       /* choose m elements from the rest */
        total += small * large;
    }

    return total;
}
