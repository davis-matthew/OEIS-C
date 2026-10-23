/*
 * A036679  a(n) = n^n - n!.
 * https://oeis.org/A036679
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy, nice
 * Formula:   a(n) = n^n - n!
 * Range:     u8 n <= 4, u16 n <= 6, u32 n <= 9, u64 n <= 16, u128 n <= 26,
 *            big any n
 * Helpers:   big_pow, factorial
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computes the OEIS sequence A036679: a(n) = n^n - n! */

#include "../utils/big_pow.h"
#include "../utils/factorial.h"

seq_t a(idx_t n)
{
    if (n == 0)
        return 0;
    seq_t p = big_pow((seq_t)n, (seq_t)n);
    seq_t f = factorial(n);
    return p - f;
}
