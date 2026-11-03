/*
 * A179060  Number of non-attacking placements of 5 rooks on an n X n board.
 * https://oeis.org/A179060
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = 120 * binom(n,5)^2
 * Range:     u8 n <= 5, u16 n <= 7, u32 n <= 16, u64 n <= 138, u128 n <= 200,
 *            big any n
 * Validated: standard, pass, 43 terms through n = 184 (2026-11-01)
 * Helpers:   binomial
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include "../utils/binomial.h"

seq_t a(idx_t n)
{
    if (n < 5)
        return 0;

    seq_t c = binomial(n, (idx_t)5);   /* C(n,5) */
    seq_t res = c * c;              /* square */
    res *= (seq_t)120;              /* multiply by 5! = 120 */
    return res;
}
