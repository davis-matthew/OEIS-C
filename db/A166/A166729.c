/*
 * A166729  Positive integers with English names ending in "t".
 * https://oeis.org/A166729
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  easy, nonn, word
 * Formula:   a(n) = 10*k + 8, where k is the (n-1)-th number of the sequence
 *            of nonnegative integers that are NOT ≡ 1 (mod 10)
 * Range:     u8 n <= 22, u16 n <= 54, u32 n <= 54, u64 n <= 54, u128 n <= 54,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

seq_t a(idx_t n)
{
    /* We assume n ≥ 1. For n == 0 the result is undefined. */
    if (n == 0)
        return (seq_t)0;

    /* Convert n to zero‑based index. */
    idx_t i = n - 1;

    /* Divide the index into groups of 9 valid k's.
       Each full group skips one integer (the one ≡ 1 mod 10). */
    idx_t block = i / 9;
    idx_t pos   = i % 9;

    /* Compute the valid k. */
    seq_t k;
    if (pos == 0)
        k = (seq_t)block * 10;               /* base of the block */
    else
        k = (seq_t)block * 10 + (seq_t)pos + 1;  /* skip the k ≡ 1 (mod 10) */

    /* Return the nth term of the sequence. */
    return (seq_t)10 * k + (seq_t)8;
}
