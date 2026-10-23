/*
 * A134369  a(n) = ((2n+1)!)^(n+1).
 * https://oeis.org/A134369
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = ((2n+1)!)^(n+1)
 * Range:     u8 n <= 1, u16 n <= 1, u32 n <= 2, u64 n <= 3, u128 n <= 4,
 *            big any n
 * Helpers:   factorial, seq_pow
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/factorial.h"
#include "../utils/seq_pow.h"

seq_t a(idx_t n)
{
    /* Compute (2n+1)! */
    idx_t m = 2 * n + 1;
    seq_t fact = factorial(m);

    /* Raise to the (n+1)-th power */
    return seq_pow(fact, n + 1);
}
