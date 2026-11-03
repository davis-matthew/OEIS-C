/*
 * A105943  a(n) = binomial(n+7,7) * binomial(n+10,7).
 * https://oeis.org/A105943
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = binomial(n+7,7) * binomial(n+10,7)
 * Range:     u8 n <= 0, u16 n <= 2, u32 n <= 11, u64 n <= 74, u128 n <= 1000,
 *            big any n
 * Validated: standard, pass, 38 terms through n = 900 (2026-11-01)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

/* multiplicative binomial C(N,7) when N >= 7 */
static inline seq_t binom7(idx_t N)
{
    seq_t r = 1;
    /* compute (N-6)(N-5)...(N)/1*2*...*7 */
    for (idx_t i = 1; i <= 7; ++i) {
        r = r * (seq_t)(N - 7 + i) / (seq_t)i;
    }
    return r;
}

seq_t a(idx_t n)
{
    /* n + 7 >= 7, n + 10 >= 10 always for n >= 0 */
    seq_t c1 = binom7(n + 7);
    seq_t c2 = binom7(n + 10);
    return c1 * c2;
}
