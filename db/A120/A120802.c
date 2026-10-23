/*
 * A120802  a(n) = 5^(n^2)+3^n.
 * https://oeis.org/A120802
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 5^(n^2) + 3^n
 * Range:     u8 n <= 1, u16 n <= 2, u32 n <= 3, u64 n <= 5, u128 n <= 7,
 *            big any n
 * Helpers:   big_pow, power3
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/big_pow.h"
#include "../utils/power3.h"

seq_t a(idx_t n)
{
    seq_t nn = (seq_t)n * (seq_t)n;          /* n^2 in seq_t */
    seq_t term1 = big_pow((seq_t)5, nn);     /* 5^(n^2) */
    seq_t term2 = power3(n);                 /* 3^n */
    return term1 + term2;
}
