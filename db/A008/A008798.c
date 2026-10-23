/*
 * A008798  Molien series for group [2,5]+ = 225.
 * https://oeis.org/A008798
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 67, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   sum_for_n
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*
 * Formula: a(n) = coefficient of x^n in (1+x^6)/((1-x^2)^2*(1-x^5))
 *           = sum_{m,k≥0, 2k+5m=n}(k+1) + sum_{m,k≥0, 2k+5m=n-6}(k+1)
 */

#include <stdint.h>
#include "../utils/sum_for_n.h"

seq_t a(idx_t n)
{
    seq_t res = sum_for_n(n);
    if (n >= 6)
        res += sum_for_n(n - 6);
    return res;
}
