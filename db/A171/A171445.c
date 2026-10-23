/*
 * A171445  Expansion of g.f. (1+z)^(24)/(1-z).
 * https://oeis.org/A171445
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = Sum_{k=0..floor(n/2)} binomial(25, n-2*k) For n >= 24 the
 *            sum equals 2^24
 * Range:     u8 n <= 1, u16 n <= 4, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   binomial
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/binomial.h"

seq_t a(idx_t n)
{
    /* m = 25 from the OEIS definition */
    const idx_t m = 25;

    /* Quick return for n >= 24: the sum of all (even or odd) binomials
       of 25 is 2^24 */
    if (n >= 24)
        return ((seq_t)1 << 24);

    seq_t sum = 0;
    idx_t k_start = 0;
    idx_t k_end   = n / 2;                    /* floor(n/2) */

    /* For n > 25, only values k with n-2k <= 25 contribute.
       k >= ceil((n-25)/2)  */
    if (n > 25)
        k_start = (n - 25 + 1) / 2;            /* ceil((n-25)/2) */

    for (idx_t k = k_start; k <= k_end; ++k) {
        idx_t r = n - 2 * k;                  /* n-2k */
        if (r > m)                            /* safety guard */
            continue;
        sum += binomial(m, r);
    }
    return sum;
}
