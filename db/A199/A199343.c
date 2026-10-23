/*
 * A199343  Least integer > n having a larger or the same digital sum as n.
 * https://oeis.org/A199343
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, base
 * Formula:   a(n) = n + 1 if n % 10 != 9 a(n) = n + 9 * 10^(d-1) otherwise,
 *            where d is the number of trailing 9s of n (equivalent to n + 9 *
 *            10^(valuation(n+1,10)-1))
 * Range:     u8 n <= 99, u16 n <= 99, u32 n <= 99, u64 n <= 99, u128 n <= 99,
 *            big any n
 * Helpers:   pow10, seq_mul
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include "../utils/pow10.h"
#include "../utils/seq_mul.h"

seq_t a(idx_t n)
{
    /* If n does not end in 9, the next integer has a larger digital sum. */
    if ((n % 10) != 9)
        return (seq_t)n + 1;

    /* Count trailing 9's in n.  n is guaranteed to be positive here because
       n % 10 == 9. */
    idx_t temp = n;
    unsigned d = 0;
    while (temp % 10 == 9) {
        ++d;
        temp /= 10;
    }

    /* d >= 1 here.  Compute the increment 9 * 10^(d-1). */
    seq_t ten_pow = pow10(d - 1);
    seq_t inc = seq_mul((seq_t)9, ten_pow);

    return (seq_t)n + inc;
}
