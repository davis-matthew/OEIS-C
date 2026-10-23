/*
 * A143462  Expansion of 1/(1 + 4*x + 8*x^2).
 * https://oeis.org/A143462
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign, easy
 * Formula:   a(0)=1, a(1)=-4, recurrence a(n)= -4*a(n-1)-8*a(n-2) (OEIS
 *            A143462)
 * Range:     i8 n <= 4, i16 n <= 9, i32 n <= 20, i64 n <= 29, i128 n <= 29,
 *            big any n
 * Helpers:   pow_seq
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/pow_seq.h"

seq_t a(idx_t n)
{
    /* Base values for n mod 4 */
    static const seq_t base[4] = {1, -4, 8, 0};

    idx_t k = n >> 2;          /* k = floor(n/4) */
    int   r = n & 3;           /* r = n mod 4 */

    /* If base is zero, the whole term is zero */
    if (base[r] == 0)
        return 0;

    /* Compute (-64)^k */
    seq_t pow64 = pow_seq((seq_t)-64, k);

    /* Result is base[r] * (-64)^k */
    return base[r] * pow64;
}
