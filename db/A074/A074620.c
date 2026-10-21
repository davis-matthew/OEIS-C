/*
 * A074620  a(n) = 6^n + 8^n.
 * https://oeis.org/A074620
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = 6^n + 8^n
 * Range:     u8 n <= 2, u16 n <= 5, u32 n <= 10, u64 n <= 19, u128 n <= 19,
 *            big any n
 * Helpers:   big_pow
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/big_pow.h"

/* Return a(n) = 6^n + 8^n for n ≥ 0.
   The function works with any seq_t type, using big_pow for
   arbitrary-precision exponentiation when supported. */
seq_t a(idx_t n)
{
    /* Convert the index to the sequence type for exponentiation. */
    seq_t exp = (seq_t)n;

    /* Compute 6^n and 8^n using the provided big_pow utility. */
    seq_t p6 = big_pow((seq_t)6, exp);
    seq_t p8 = big_pow((seq_t)8, exp);

    /* Sum the two powers to obtain a(n). */
    return p6 + p8;
}
