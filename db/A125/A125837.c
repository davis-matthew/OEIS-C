/*
 * A125837  a(n) = 6*(8^n - 1)/7.
 * https://oeis.org/A125837
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy, base
 * Formula:   a(n) = 6*(8^n - 1)/7
 * Range:     u8 n <= 2, u16 n <= 5, u32 n <= 10, u64 n <= 21, u128 n <= 42,
 *            big any n
 * Helpers:   big_pow
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/big_pow.h"

seq_t a(idx_t n)
{
    /* Compute 8^n as a seq_t.  For arbitrary‑precision types or
       built‑in uX types big_pow will return an exact value. */
    seq_t pow8 = big_pow((seq_t)8, (seq_t)n);

    /* 8^n - 1 is always divisible by 7.  Divide first then multiply
       by 6 to avoid overflow when seq_t is a 64‑bit unsigned type. */
    seq_t result = (pow8 - (seq_t)1) / (seq_t)7 * (seq_t)6;
    return result;
}
