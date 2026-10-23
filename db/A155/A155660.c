/*
 * A155660  a(n) = 9^n - 7^n + 1.
 * https://oeis.org/A155660
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 9^n - 7^n + 1
 * Range:     u8 n <= 2, u16 n <= 5, u32 n <= 10, u64 n <= 20, u128 n <= 40,
 *            big any n
 * Helpers:   big_pow
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/big_pow.h"

seq_t a(idx_t n)
{
    /* Compute 9^n and 7^n using the provided big exponentiation routine.
       seq_t is assumed to support construction from integer constants
       and the usual arithmetic operators. */
    seq_t p9 = big_pow((seq_t)9, (seq_t)n);
    seq_t p7 = big_pow((seq_t)7, (seq_t)n);
    return p9 - p7 + (seq_t)1;
}
