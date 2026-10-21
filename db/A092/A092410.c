/*
 * A092410  a(n) = moebius(n)+moebius(n+1).
 * https://oeis.org/A092410
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  sign, easy
 * Formula:   a(n) = mu(n) + mu(n+1) (mu = Möbius function)
 * Range:     i8 n <= 10000, i16 n <= 10000, i32 n <= 10000, i64 n <= 10000,
 *            i128 n <= 10000, big any n
 * Helpers:   mobius
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/mobius.h"

seq_t a(idx_t n)
{
    /* For n < 1 the sequence is undefined; return 0 here */
    if (n == 0)
        return 0;

    /* Compute Möbius values for n and n+1 and sum them */
    seq_t val_n   = mobius(n);
    seq_t val_np1 = mobius(n + 1);

    return val_n + val_np1;
}
