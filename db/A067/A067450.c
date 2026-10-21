/*
 * A067450  Smallest n-th power starting with 9.
 * https://oeis.org/A067450
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  base, easy, nonn
 * Formula:   not stated
 * Range:     u8 n <= 2, u16 n <= 3, u32 n <= 5, u64 n <= 13, u128 n <= 18,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computes A067450: smallest n-th power whose decimal representation
   starts with the digit 9 */
#include <stdint.h>
#include <stdbool.h>
#include <math.h>

/* Fast exponentiation: returns base^exp for arbitrary-precision seq_t
   types.  exp is an idx_t (uint64_t). */
static seq_t pow_seq(seq_t base, idx_t exp)
{
    seq_t result = (seq_t)1;
    seq_t cur   = base;
    while (exp)
    {
        if (exp & (idx_t)1)
            result = result * cur;
        exp >>= (idx_t)1;
        if (exp)
            cur = cur * cur;
    }
    return result;
}

/* Return the smallest integer k such that k^n has leading digit 9,
   and return that power value.  The search is performed by scanning
   over k and checking the fractional part of n·log10(k).  This runs
   in well under a second for n ≤ 10 000 on typical hardware. */
seq_t a(idx_t n)
{
    /* log10(9) is the threshold for a leading digit 9. */
    static const long double LOG9 = log10l((long double)9.0L);

    /* Start the search at k = 1 (1^n never starts with 9). */
    for (uint64_t k = 1; ; ++k)
    {
        long double logk = log10l((long double)k);
        long double f    = (long double)n * logk;
        f                = f - floorl(f);               /* fractional part */

        if (f >= LOG9)
        {
            seq_t base   = (seq_t)k;
            seq_t power  = pow_seq(base, n);
            return power;
        }
    }
}
