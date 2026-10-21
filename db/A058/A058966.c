/*
 * A058966  a(3) = 1, otherwise a(n) = n*2^(n-3) - 2^(n-2) - 2.
 * https://oeis.org/A058966
 *
 * Offset:    3    (a(3) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 8, u16 n <= 15, u32 n <= 30, u64 n <= 61, u128 n <= 124,
 *            big any n
 * Helpers:   pow2_shift
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/pow2_shift.h"
/*
 * Formula: a(3) = 1, otherwise a(n) = n*2^(n-3) - 2^(n-2) - 2   (n ≥ 4)
 * This implementation computes 2^(n-3) by repeated left shifts.
 * For n < 3 the value 0 is returned (the offset of the sequence is 3).
 */

seq_t a(idx_t n)
{
    if (n < 3)          /* below the offset: value is unspecified; return 0 */
        return 0;
    if (n == 3)         /* special case defined in the OEIS entry */
        return 1;

    /* For n ≥ 4 compute the formula */
    seq_t two_n_minus3 = pow2_shift(n - 3);       /* 2^(n-3) */
    seq_t two_n_minus2 = two_n_minus3 << 1;       /* 2^(n-2) */

    /* Evaluate: n*2^(n-3) - 2^(n-2) - 2 */
    seq_t result = (seq_t)n * two_n_minus3;
    result -= two_n_minus2;
    result -= 2;

    return result;
}
