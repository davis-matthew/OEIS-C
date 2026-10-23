/*
 * A171764  Binary expansion of numbers in A171763.
 * https://oeis.org/A171764
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   a(n) = A007088(A171763(n))
 * Range:     u8 n <= 1, u16 n <= 7, u32 n <= 255, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   floor_log2_uint64
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include "../utils/floor_log2_uint64.h"

seq_t a(idx_t n)
{
    /* Find the length L of the binary string:
       2^(L-2)-1 >= n  <=> 2^(L-2) >= n+1
       Let k = ceil(log2(n+1)), then L = k+2.                      */
    idx_t n_plus_one = n + 1;
    uint32_t k = floor_log2_uint64(n_plus_one);
    if (((seq_t)1 << k) < n_plus_one) {
        ++k;
    }
    idx_t L = k + 2;                     /* length of the binary string */

    /* The first binomial of this length starts at index 2^(L-3). */
    idx_t first_index = ((idx_t)1 << (L - 3));
    idx_t m = n - first_index;           /* offset within this length group */

    /* Build the decimal digits of the binary string. */
    seq_t result = 0;
    for (idx_t pos = L; pos > 0; --pos) {
        idx_t i = pos - 1;               /* index of the bit (MSB= L-1) */
        int bit;
        if (i == L - 1) {
            bit = 1;                       /* leading 1 */
        } else if (i == L - 2) {
            bit = 0;                       /* second bit 0 */
        } else if (i == 0) {
            bit = 1;                       /* trailing 1 */
        } else {
            bit = (int)((m >> (i - 1)) & 1);   /* bits from m */
        }
        result = result * 10 + bit;
    }

    return result;
}
