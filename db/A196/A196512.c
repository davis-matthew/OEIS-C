/*
 * A196512  Expansion of g.f. (1-9*x)/(1-28*x).
 * https://oeis.org/A196512
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 19*28^(n-1) for n>0, a(0)=1
 * Range:     u8 n <= 1, u16 n <= 3, u32 n <= 6, u64 n <= 13, u128 n <= 26,
 *            big any n
 * Helpers:   seq_pow
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/seq_pow.h"   /* Provides seq_pow(seq_t, idx_t) */

seq_t a(idx_t n)
{
    if (n == 0)
        return 1;                      /* a(0) = 1 */
    /* Compute 19 * 28^(n-1) using arbitrary‑precision exponentiation */
    return 19 * seq_pow((seq_t)28, n - 1);
}
