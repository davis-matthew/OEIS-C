/*
 * A089519  Fourth column (k=6) of array A078741 ((3,3)-Stirling2).
 * https://oeis.org/A089519
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 20*(6*5*4)^n - 30*(5*4*3)^n + 12*(4*3*2)^n - (3*2*1)^n
 * Range:     u8 n <= 0, u16 n <= 1, u32 n <= 4, u64 n <= 8, u128 n <= 11,
 *            big any n
 * Helpers:   seq_pow_unsigned
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/seq_pow_unsigned.h"

seq_t a(idx_t n)
{
    /* Pre‑compute the constant bases as seq_t */
    const seq_t base120 = (seq_t)120;  /* 6*5*4 */
    const seq_t base60  = (seq_t)60;   /* 5*4*3 */
    const seq_t base24  = (seq_t)24;   /* 4*3*2 */
    const seq_t base6   = (seq_t)6;    /* 3*2*1 */

    /* Compute the powers */
    seq_t p120 = seq_pow_unsigned(base120, n);
    seq_t p60  = seq_pow_unsigned(base60,  n);
    seq_t p24  = seq_pow_unsigned(base24,  n);
    seq_t p6   = seq_pow_unsigned(base6,   n);

    /* Assemble the final value */
    seq_t a_val = (seq_t)20 * p120 - (seq_t)30 * p60 + (seq_t)12 * p24 - p6;
    return a_val;
}
