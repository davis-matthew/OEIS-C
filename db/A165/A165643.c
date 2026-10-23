/*
 * A165643  Number of ways to assemble an n-cube from 2n labeled (n-1)-cubes
 *          with labeled vertices, where left-handed and right-handed
 *          counterparts are considered equivalent.
 * https://oeis.org/A165643
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = ((2n-2)!!)^ (2n-1) * (2n-1)!
 * Range:     u8 n <= 2, u16 n <= 2, u32 n <= 3, u64 n <= 4, u128 n <= 5,
 *            big any n
 * Helpers:   factorial, seq_pow
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/factorial.h"
#include "../utils/seq_pow.h"

seq_t a(idx_t n)
{
    /* n >= 1 (sequence is defined from offset 1) */
    idx_t nn = (idx_t)n;

    /* (n-1)! */
    seq_t fact_nm1 = factorial(nn - 1);

    /* (2n-1)! */
    seq_t fact_2n_1 = factorial(2 * nn - 1);

    /* 2^(n-1) */
    seq_t twos = seq_pow((seq_t)2, nn - 1);

    /* (2n-2)!! = 2^(n-1) * (n-1)! */
    seq_t base = twos * fact_nm1;

    /* exponent (2n-1) */
    idx_t exp = 2 * nn - 1;

    /* ((2n-2)!!)^(2n-1) */
    seq_t base_pow = seq_pow(base, exp);

    /* final result */
    seq_t result = base_pow * fact_2n_1;

    return result;
}
