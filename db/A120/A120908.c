/*
 * A120908  Sum of the lengths of the drops in all ternary words of length n
 *          on {0,1,2}. The drops of a ternary word on {0,1,2} are the
 *          subwords 10,20 and 21, their lengths being the differences 1, 2
 *          and 1, respectively.
 * https://oeis.org/A120908
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 4*(n-1)*3^(n-2)
 * Range:     u8 n <= 4, u16 n <= 8, u32 n <= 18, u64 n <= 37, u128 n <= 77,
 *            big any n
 * Helpers:   big_pow
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/big_pow.h"

seq_t a(idx_t n)
{
    /* a(1) = 0, a(2) = 4 are covered by the n <= 2 branch */
    if (n <= 2)
        return (seq_t)4 * (seq_t)(n - 1);

    /* Compute 3^(n-2) */
    seq_t pow3 = big_pow((seq_t)3, (seq_t)(n - 2));

    /* Return 4*(n-1)*pow3 */
    return (seq_t)4 * (seq_t)(n - 1) * pow3;
}
