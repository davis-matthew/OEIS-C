/*
 * A006413  Number of nonseparable tree-rooted planar maps with n + 4 edges
 *          and 5 vertices.
 * https://oeis.org/A006413
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 5*binomial(n+6,7) + 170*binomial(n+6,8) +
 *            1440*binomial(n+6,9) + 4906*binomial(n+6,10) +
 *            7927*binomial(n+6,11) + 6090*binomial(n+6,12) +
 *            1794*binomial(n+6,13)
 * Range:     u8 n <= 2, u16 n <= 4, u32 n <= 14, u64 n <= 93, u128 n <= 1000,
 *            big any n
 * Helpers:   binom
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/binom.h"

seq_t a(idx_t n)
{
    idx_t m = n + 6;
    seq_t result = 0;
    result += (seq_t)5 * binom(m, 7);
    result += (seq_t)170 * binom(m, 8);
    result += (seq_t)1440 * binom(m, 9);
    result += (seq_t)4906 * binom(m, 10);
    result += (seq_t)7927 * binom(m, 11);
    result += (seq_t)6090 * binom(m, 12);
    result += (seq_t)1794 * binom(m, 13);
    return result;
}
