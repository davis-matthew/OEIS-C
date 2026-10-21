/*
 * A212403  Number of binary arrays of length 2*n+1 with no more than n ones
 *          in any length 2n subsequence (=50% duty cycle).
 * https://oeis.org/A212403
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 4^n + C(2*n-1, n)
 * Range:     u8 n <= 3, u16 n <= 7, u32 n <= 15, u64 n <= 31, u128 n <= 63,
 *            big any n
 * Helpers:   binom, pow2
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include "../utils/pow2.h"
#include "../utils/binom.h"

/* Returns the nth term of OEIS sequence A212403.
   Indexing follows OEIS offset: a(1) is the first term. */
seq_t a(idx_t n)
{
    /* For n < 1 the behaviour is undefined; not needed per specifications. */
    /* 4^n = 2^(2n) */
    seq_t pow4 = pow2((idx_t)(n * 2ULL));
    /* binomial(2*n-1, n) */
    seq_t bin = binom((idx_t)(n * 2ULL - 1), n);
    return pow4 + bin;
}
