/*
 * A140453  Half the number of n X n binary matrices, symmetric under
 *          horizontal and vertical reflection, having equal row sums.
 * https://oeis.org/A140453
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   Let m = floor(n/2), r = ceil(n/2) = (n+1)/2. Then a(n) = (n
 *            even) : (1/2) * Σ_{k=0}^{m} C(m,k)^r a(n) = (n odd) :
 *            Σ_{k=0}^{m} C(m,k)^r (for n=1, m=0, r=1 this gives a(1)=1)
 * Range:     u8 n <= 7, u16 n <= 9, u32 n <= 13, u64 n <= 18, u128 n <= 24,
 *            big any n
 * Helpers:   binom, seq_pow
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>
#include "../utils/binom.h"
#include "../utils/seq_pow.h"

seq_t a(idx_t n)
{
    /* m = floor(n/2), r = ceil(n/2) */
    idx_t m = n / 2;          /* floor division */
    idx_t r = (n + 1) / 2;    /* ceil(n/2) */

    seq_t sum = 0;            /* accumulate Σ C(m,k)^r */

    for (idx_t k = 0; k <= m; ++k)
    {
        seq_t bin = binom(m, k);           /* C(m,k) */
        seq_t power = seq_pow(bin, r);     /* C(m,k)^r */
        sum += power;                      /* add to sum */
    }

    if ((n & 1ULL) == 0)          /* even n -> divide by 2 */
    {
        sum /= 2;
    }

    return sum;
}
