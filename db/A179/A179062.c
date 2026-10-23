/*
 * A179062  Number of non-attacking placements of 7 rooks on an n X n board.
 * https://oeis.org/A179062
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = 7!*binomial(n,7)^2
 * Range:     u8 n <= 6, u16 n <= 7, u32 n <= 12, u64 n <= 46, u128 n <= 200,
 *            big any n
 * Helpers:   binomial
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/binomial.h"

seq_t a(idx_t n)
{
    if (n < 7) return 0;
    seq_t c = binomial(n, 7);
    seq_t res = (seq_t)5040 * c;
    res *= c;
    return res;
}
