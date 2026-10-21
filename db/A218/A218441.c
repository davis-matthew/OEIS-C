/*
 * A218441  a(n) = A000108(n)*A001764(n).
 * https://oeis.org/A218441
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = catalan(n) * fusc(n) where catalan(n) =
 *            binom(2n,n)/(n+1), * fusc(n) = binom(3n,n)/(2n+1)
 * Range:     u8 n <= 3, u16 n <= 5, u32 n <= 9, u64 n <= 16, u128 n <= 30,
 *            big any n
 * Helpers:   binom
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/binom.h"

seq_t a(idx_t n)
{
    seq_t catalan = binom(2 * n, n) / (seq_t)(n + 1);
    seq_t fusc    = binom(3 * n, n) / (seq_t)(2 * n + 1);
    return catalan * fusc;
}
