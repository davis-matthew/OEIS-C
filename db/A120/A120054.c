/*
 * A120054  a(n) = binomial(n+3,4)*4^4.
 * https://oeis.org/A120054
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = binom(n+3,4) * 4^4 (OEIS A120054)
 * Range:     u8 none, u16 n <= 7, u32 n <= 32, u64 n <= 32, u128 n <= 32,
 *            big any n
 * Helpers:   binom
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/binom.h"

seq_t a(idx_t n)
{
    /* Undefined for n < 1; return 0 as a safe fallback. */
    if (n == 0)
        return 0;

    seq_t comb = binom(n + 3, 4);   /* C(n+3, 4) */
    return comb * (seq_t)256;       /* 4^4 = 256 */
}
