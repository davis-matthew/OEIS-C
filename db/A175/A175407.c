/*
 * A175407  a(n) = the least integer k such that (1+1/n)^k >= 3.
 * https://oeis.org/A175407
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = ceil( ln(3) / ln(1 + 1/n) ) (n ≥ 1)
 * Range:     u8 n <= 231, u16 n <= 2000, u32 n <= 2000, u64 n <= 2000,
 *            u128 n <= 2000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <math.h>

seq_t a(idx_t n)
{
    /* n must be ≥ 1; behaviour for n = 0 is irrelevant for the offset */
    if (n == 0)
        return 0;

    const long double ln3 = logl(3.0L);
    /* compute ln(1 + 1/n) using long double arithmetic */
    const long double ln1pn = logl(1.0L + 1.0L / (long double)n);
    /* compute the real number k */
    const long double k_real = ln3 / ln1pn;
    /* take the ceiling; cast to idx_t then to seq_t */
    idx_t k = (idx_t)ceill(k_real);
    return (seq_t)k;
}
