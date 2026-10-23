/*
 * A179063  Number of non-attacking placements of 8 rooks on an n X n board.
 * https://oeis.org/A179063
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = 40320 * binomial(n,8)^2 a(n) = 8! * C(n,8)^2 For n < 8,
 *            a(n) = 0
 * Range:     u8 n <= 7, u16 n <= 8, u32 n <= 11, u64 n <= 34, u128 n <= 200,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

seq_t a(idx_t n)
{
    if (n < 8)
        return 0;

    /* Compute C(n,8) multiplicatively using unsigned __int128
       to avoid overflow during intermediate multiplications. */
    unsigned __int128 binom = 1;
    for (idx_t i = 1; i <= 8; ++i) {
        binom = binom * (n - 8 + i);
        binom = binom / i;
    }

    /* The final result: 40320 * binom^2 */
    unsigned __int128 result = binom * binom * 40320U;
    return (seq_t)result;
}
