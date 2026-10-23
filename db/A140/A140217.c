/*
 * A140217  Half the number of n X n binary matrices, symmetric under
 *          horizontal reflection, having equal row sums.
 * https://oeis.org/A140217
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   For n odd: a(n) = Σ_{k=0}^{⌊n/2⌋} C(⌊n/2⌋,k)^n For n even: a(n)
 *            = (1/2) * Σ_{k=0}^{⌊n/2⌋} C(⌊n/2⌋,k)^n where C(a,b) is the
 *            binomial coefficient. Reference: A140217 – Half the number of
 *            n×n binary matrices that are symmetric under column reversal and
 *            have equal row sums
 * Range:     u8 n <= 5, u16 n <= 7, u32 n <= 9, u64 n <= 13, u128 n <= 18,
 *            big any n
 * Helpers:   big_pow, binomial
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include "../utils/binomial.h"
#include "../utils/big_pow.h"

seq_t a(idx_t n)
{
    if (n == 0) return 0;                 /* not used (offset 1) */
    idx_t m = n / 2;                       /* floor(n/2) */
    seq_t sum = 0;                         /* accumulate Σ C(m,k)^n  */
    for (idx_t k = 0; k <= m; ++k)
    {
        seq_t bin = binomial(m, k);        /* C(m,k) */
        seq_t p   = big_pow(bin, (seq_t)n);/* bin^n */
        sum += p;
    }
    if ((n & 1) == 0)                     /* n even: divide by 2 */
        sum >>= 1;                        /* safe: sum is even */
    return sum;
}
