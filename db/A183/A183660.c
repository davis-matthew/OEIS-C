/*
 * A183660  Number of (n+1) X 8 0..5 arrays with every 2 X 2 subblock summing
 *          to 10.
 * https://oeis.org/A183660
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 6*6^n + 1270*5^n + 24200*4^n + 158010*3^n + 532228*2^n +
 *            963902
 * Range:     u8 none, u16 none, u32 n <= 8, u64 n <= 22, u128 n <= 48,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

/* Compute base^exp for small integer base.  Assumes exp is non‑negative. */
static seq_t pow_small(seq_t base, idx_t exp)
{
    seq_t result = 1;
    for (idx_t i = 0; i < exp; ++i)
        result *= base;
    return result;
}

seq_t a(idx_t n)
{
    /* Values for n >= 1, as defined by the OEIS formula. */
    seq_t term1 = (seq_t)6 * pow_small((seq_t)6, n);
    seq_t term2 = (seq_t)1270 * pow_small((seq_t)5, n);
    seq_t term3 = (seq_t)24200 * pow_small((seq_t)4, n);
    seq_t term4 = (seq_t)158010 * pow_small((seq_t)3, n);
    seq_t term5 = (seq_t)532228 * pow_small((seq_t)2, n);
    seq_t term6 = (seq_t)963902;
    return term1 + term2 + term3 + term4 + term5 + term6;
}
