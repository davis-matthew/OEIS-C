/*
 * A211221  For any partition of n consider the product of the sigma of each
 *          element. Sequence gives the maximum of such values.
 * https://oeis.org/A211221
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(1)=1; for n>1, a(n)=3^(n/2) if n even, else a(n)=4*3^((n-3)/2)
 * Range:     u8 n <= 10, u16 n <= 20, u32 n <= 37, u64 n <= 37, u128 n <= 37,
 *            big any n
 * Helpers:   pow_int
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include "../utils/pow_int.h"

seq_t a(idx_t n)
{
    if (n == 1)
        return (seq_t)1;

    if ((n & 1) == 0) {              /* even n */
        idx_t k = n >> 1;            /* k = n/2 */
        return pow_int((seq_t)3, k);
    }
    else {                            /* odd n */
        idx_t k = (n - 3) >> 1;      /* k = (n-3)/2 */
        return (seq_t)4 * pow_int((seq_t)3, k);
    }
}
