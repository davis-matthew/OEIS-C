/*
 * A145627  Numerator of the polynomial A_l(x) = sum_{d=1..l-1} x^(l-d)/d for
 *          index l=2n+1 evaluated at x=10.
 * https://oeis.org/A145627
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  frac, nonn
 * Formula:   a(n) = numerator of A_{2n+1}(10), where * A_l(x) = Σ_{d=1}^{l-1}
 *            x^{l-d}/d * * Computes the fraction A_{2n+1}(10) = N/D in lowest
 *            terms * and returns the numerator N. * * The algorithm uses the
 *            recurrence * * f[l+1] = 10*f[l] + 10/l (l ≥ 2) * * where f[l] =
 *            A_l(10). The fraction is maintained as...
 * Range:     u8 n <= 1, u16 n <= 1, u32 n <= 3, u64 n <= 7, u128 n <= 11,
 *            big any n
 * Helpers:   gcd
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include "../utils/gcd.h"

seq_t a(idx_t n)
{
    /* l = 2*n + 1  (index of the polynomial) */
    idx_t l = ((idx_t)2 * n) + 1;

    /* f[2] is A_2(10) = 10 */
    seq_t N = (seq_t)10;
    seq_t D = (seq_t)1;
    idx_t cur_l = 2;      /* current polynomial index */

    /* Build up to the desired l */
    while (cur_l < l)
    {
        /* Compute: new_N = 10 * (N*cur_l + D)   new_D = D*cur_l  */
        seq_t temp = N * (seq_t)cur_l + D;   /* N*cur_l + D   */
        seq_t new_N = temp * (seq_t)10;      /* multiply by 10 */
        seq_t new_D = D * (seq_t)cur_l;       /* D*cur_l       */

        /* Reduce the fraction to lowest terms */
        seq_t g = gcd(new_N, new_D);
        new_N /= g;
        new_D /= g;

        /* Advance to next l */
        N = new_N;
        D = new_D;
        cur_l++;
    }

    /* Return the numerator of A_{2n+1}(10) */
    return N;
}
