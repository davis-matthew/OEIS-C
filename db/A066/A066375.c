/*
 * A066375  a(n) = 6*binomial(n,4) + 3*binomial(n,3) + 4*binomial(n,2) - n +
 *          2.
 * https://oeis.org/A066375
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 6*binomial(n,4) + 3*binomial(n,3) + 4*binomial(n,2) - n +
 *            2
 * Range:     u8 n <= 6, u16 n <= 23, u32 n <= 363, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   binom
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/binom.h"

seq_t a(idx_t n)
{
    /* For n < 1 the definition is not required; return 0. */
    if (n < 1) return 0;
    seq_t sn = (seq_t)n;
    seq_t c2 = binom(n, 2);
    seq_t c3 = binom(n, 3);
    seq_t c4 = binom(n, 4);
    seq_t res = 0;
    res += 6 * c4;
    res += 3 * c3;
    res += 4 * c2;
    res -= sn;
    res += 2;
    return res;
}
