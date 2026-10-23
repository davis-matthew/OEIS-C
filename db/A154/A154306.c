/*
 * A154306  a(n) = (n+1)^3*(3+n)!/6.
 * https://oeis.org/A154306
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = (n+1)^3 * (3+n)! / 6
 * Range:     u8 n <= 1, u16 n <= 3, u32 n <= 7, u64 n <= 15, u128 n <= 28,
 *            big any n
 * Helpers:   seq_mul
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computes the OEIS sequence A154306. */

#include "../utils/seq_mul.h"   /* provides seq_mul */

seq_t a(idx_t n)
{
    /* factorial(n+3) / 6 is the product of integers 4 .. n+3  (empty product = 1) */
    seq_t fact = 1;
    idx_t limit = n + 3;
    for (idx_t i = 4; i <= limit; ++i)
        fact = seq_mul(fact, (seq_t)i);

    /* (n+1)^3 */
    seq_t base  = (seq_t)(n + 1);
    seq_t cube  = seq_mul(seq_mul(base, base), base);

    return seq_mul(fact, cube);
}
