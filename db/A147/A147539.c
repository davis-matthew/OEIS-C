/*
 * A147539  Numbers whose binary representation is the concatenation of n 1's,
 *          2n-1 digits 0 and n 1's.
 * https://oeis.org/A147539
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  base, easy, nonn
 * Formula:   a(n) = 2^n - 1 + 2^(4n-1) - 2^(3n-1)
 * Range:     u8 n <= 2, u16 n <= 4, u32 n <= 8, u64 n <= 16, u128 n <= 32,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

seq_t a(idx_t n)
{
    /* The sequence is defined for n ≥ 1; we return 0 for n = 0 to
       avoid undefined left shift of a negative exponent. */
    if (n == 0)
        return 0;

    /* 2^n */
    seq_t pow_n = ((seq_t)1 << n);
    /* (2^n) - 1 */
    seq_t t = pow_n - 1;
    /* (t) << (3n-1)  =>  (2^n-1) * 2^(3n-1) */
    seq_t shifted = t << (3ULL * n - 1);
    /* Final result: (2^n-1) + shifted */
    return shifted + t;
}
