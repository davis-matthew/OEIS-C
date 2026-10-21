/*
 * A072252  Numbers n for which one step of the Collatz iteration (3n+1)/2^r
 *          gives rise to values 41,35,29,23,17,11, and 5 for r=1,3,5,..,13.
 * https://oeis.org/A072252
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, fini, full
 * Formula:   a(n) = ((82 - 12 * (n - 1)) * 4^(n - 1) - 1) / 3
 * Range:     u8 n <= 1, u16 n <= 7, u32 n <= 7, u64 n <= 7, u128 n <= 7,
 *            big any n
 * Helpers:   big_pow
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/big_pow.h"

seq_t a(idx_t n)
{
    /* Convert the 1‑based index to a 0‑based exponent. */
    idx_t m = n - 1u;

    /* 4^m */
    seq_t pow4 = big_pow((seq_t)4, (seq_t)m);

    /* coefficient = 82 - 12*m */
    seq_t coeff = (seq_t)82 - (seq_t)12 * (seq_t)m;

    /* (coeff * pow4 - 1) / 3 */
    seq_t val = coeff * pow4;
    val = val - (seq_t)1;
    val = val / (seq_t)3;

    return val;
}
