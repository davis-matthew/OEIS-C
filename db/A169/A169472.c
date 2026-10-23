/*
 * A169472  Number of reduced words of length n in Coxeter group on 27
 *          generators S_i with relations (S_i)^2 = (S_i S_j)^33 = I.
 * https://oeis.org/A169472
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(0) = 1, a(n) = 27 * 26^(n-1) for n >= 1
 * Range:     u8 n <= 1, u16 n <= 3, u32 n <= 6, u64 n <= 13, u128 n <= 15,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>     /* for size_t */

seq_t a(idx_t n)
{
    if (n == 0)          /* base case */
        return (seq_t)1;

    /* compute 26^(n-1) then multiply by 27 */
    seq_t power = (seq_t)1;
    idx_t exp = n - 1;            /* n >= 1, so n-1 fits in idx_t */
    for (idx_t i = 0; i < exp; ++i)
        power *= (seq_t)26;

    power *= (seq_t)27;
    return power;
}
