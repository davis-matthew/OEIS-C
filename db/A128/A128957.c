/*
 * A128957  a(n) is equal to the number of positive integers m less than or
 *          equal to 10^n such that m is not divisible by at least one of the
 *          primes 2,5 and is not divisible by at least one of the primes 3,7.
 * https://oeis.org/A128957
 *
 * Offset:    2    (a(2) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 10^n - ⌊10^n/10⌋ - ⌊10^n/21⌋ + ⌊10^n/210⌋
 * Range:     u8 n <= 2, u16 n <= 4, u32 n <= 9, u64 n <= 19, u128 n <= 38,
 *            big any n
 * Helpers:   pow10
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include "../utils/pow10.h"

seq_t a(idx_t n)
{
    /* The sequence is defined only for n >= 2. For smaller indices return 0 */
    if (n < 2)
        return 0;

    seq_t p = pow10(n);          /* 10^n */
    seq_t res = p;
    res -= p / 10;               /* floor(10^n / 10)   -> 9 * 10^{n-1} */
    res -= p / 21;               /* floor(10^n / 21)   ->  (10^n) / 21 truncated */
    res += p / 210;              /* floor(10^n / 210)  -> (10^n) / 210 truncated */

    return res;
}
