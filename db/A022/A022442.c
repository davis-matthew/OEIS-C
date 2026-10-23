/*
 * A022442  a(n) = c(n) + c(n-1) where c is the sequence of numbers not in a.
 * https://oeis.org/A022442
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(0) = 2; * for n>0, a(n) = b(n) where * b(0) = 0, * b(2n) =
 *            -b(n) + 9n + 3, * b(2n+1) = -b(n) + 9n + 6 - 2[n==0]. * The
 *            recurrence is evaluated recursively
 * Range:     u8 n <= 84, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   b_rec
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/b_rec.h"

seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)2;          /* first term as given in OEIS */
    return b_rec(n);                      /* use recurrence for n>0      */
}
