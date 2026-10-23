/*
 * A145664  a(n) = numerator of polynomial of genus 1 and level n for m = 6 =
 *          A[1,n](6).
 * https://oeis.org/A145664
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  frac, nonn
 * Formula:   a(n) = numerator of Σ_{d=1}^{n-1} 6^(n-d) / d
 * Range:     u8 n <= 3, u16 n <= 6, u32 n <= 9, u64 n <= 18, u128 n <= 18,
 *            big any n
 * Helpers:   big_pow, gcd
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include "../utils/big_pow.h"
#include "../utils/gcd.h"

seq_t a(idx_t n)
{
    if (n <= 1)
        return 0;

    seq_t num = 0;
    seq_t den = 1;

    /* start with 6^(n-1) */
    seq_t pow = big_pow((seq_t)6, (seq_t)(n - 1));

    for (idx_t d = 1; d <= n - 1; ++d)
    {
        /* term = pow / d */
        seq_t term_num = pow;
        seq_t term_den = (seq_t)d;

        /* add term to fraction num/den */
        seq_t new_num = num * term_den + term_num * den;
        seq_t new_den = den * term_den;

        seq_t g = gcd(new_num, new_den);
        num   = new_num / g;
        den   = new_den / g;

        /* prepare next power: divide by 6 */
        pow /= (seq_t)6;
    }

    return num;
}
