/*
 * A170831  a(n) = 2^(floor(n/2))+2^(floor(n/2)-1)-2^(floor((n-1)/3)).
 * https://oeis.org/A170831
 *
 * Offset:    2    (a(2) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 2^(floor(n/2)) + 2^(floor(n/2)-1) - 2^(floor((n-1)/3))
 * Range:     u8 n <= 15, u16 n <= 31, u32 n <= 44, u64 n <= 44, u128 n <= 44,
 *            big any n
 * Helpers:   pow2
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include "../utils/pow2.h"

seq_t a(idx_t n)
{
    if (n < 2) return 0;          /* behaviour below offset is irrelevant */
    idx_t k1     = n / 2;          /* floor(n/2) */
    idx_t k1m    = k1 - 1;         /* floor(n/2)-1, safe because n>=2 */
    idx_t k3     = (n - 1) / 3;    /* floor((n-1)/3) */
    seq_t term1  = pow2(k1);
    seq_t term2  = pow2(k1m);
    seq_t term3  = pow2(k3);
    return term1 + term2 - term3;
}
