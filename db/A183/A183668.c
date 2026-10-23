/*
 * A183668  Number of (n+1) X 6 0..6 arrays with every 2 X 2 subblock summing
 *          to 12.
 * https://oeis.org/A183668
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 7*7^n + 372*6^n + 3010*5^n + 10808*4^n + 24486*3^n +
 *            39004*2^n + 39962
 * Range:     u8 none, u16 none, u32 n <= 8, u64 n <= 21, u128 n <= 44,
 *            big any n
 * Helpers:   seq_pow
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/seq_pow.h"

/* Compute a(n) for OEIS A183668: Number of (n+1)×6 0..6 arrays with every 2×2 subblock summing to 12. */
seq_t a(idx_t n)
{
    /* n is 1-based index.  The formula is valid for all n ≥ 1. */
    seq_t p7 = seq_pow((seq_t)7, n);
    seq_t p6 = seq_pow((seq_t)6, n);
    seq_t p5 = seq_pow((seq_t)5, n);
    seq_t p4 = seq_pow((seq_t)4, n);
    seq_t p3 = seq_pow((seq_t)3, n);
    seq_t p2 = seq_pow((seq_t)2, n);

    return ((seq_t)7)      * p7
         + ((seq_t)372)    * p6
         + ((seq_t)3010)   * p5
         + ((seq_t)10808)  * p4
         + ((seq_t)24486)  * p3
         + ((seq_t)39004)  * p2
         + ((seq_t)39962);
}
