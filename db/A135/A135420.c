/*
 * A135420  a(n) = 4^n * 5^(n^2).
 * https://oeis.org/A135420
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 4^n * 5^(n^2)
 * Range:     u8 n <= 1, u16 n <= 2, u32 n <= 3, u64 n <= 4, u128 n <= 7,
 *            big any n
 * Helpers:   big_pow, seq_mul
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/big_pow.h"
#include "../utils/seq_mul.h"

seq_t a(idx_t n)
{
    /* Compute 4^n. */
    seq_t pow4 = big_pow((seq_t)4, (seq_t)n);

    /* Compute 5^(n^2).  n^2 is calculated in seq_t to keep the
       computation exact for arbitrary‑precision builds. */
    seq_t exp_n2 = (seq_t)n * (seq_t)n;
    seq_t pow5 = big_pow((seq_t)5, exp_n2);

    /* Return the product. */
    return seq_mul(pow4, pow5);
}
