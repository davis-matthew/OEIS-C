/*
 * A104679  a(n) = C(n+5,5)*C(n+10,5).
 * https://oeis.org/A104679
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = C(n+5,5)*C(n+10,5)
 * Range:     u8 n <= 0, u16 n <= 2, u32 n <= 18, u64 n <= 214, u128 n <= 1000,
 *            big any n
 * Validated: standard, pass, 41 terms through n = 981 (2026-11-01)
 * Helpers:   binomial
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <sys/types.h>
#include "../utils/binomial.h"

seq_t a(idx_t n)
{
    return binomial(n + 5, 5) * binomial(n + 10, 5);
}
