/*
 * A123051  a(2*n-1) = (4*n-3)^(4*n-2) and a(2*n) = (4*n)^(4*n-1), n=1,2,...
 * https://oeis.org/A123051
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(2*n-1) = (4*n-3)^(4*n-2), a(2*n) = (4*n)^(4*n-1)
 * Range:     u8 n <= 2, u16 n <= 3, u32 n <= 5, u64 n <= 8, u128 n <= 13,
 *            big any n
 * Helpers:   big_pow
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/big_pow.h"

seq_t a(idx_t n)
{
    if (n == 0) return 0; /* n is always >= 1 in valid calls */
    if (n & 1) {
        /* odd index: n = 2*k-1 */
        idx_t k = (n + 1) >> 1;
        seq_t base = (seq_t)(4 * k - 3);
        seq_t exp  = (seq_t)(4 * k - 2);
        return big_pow(base, exp);
    } else {
        /* even index: n = 2*k */
        idx_t k = n >> 1;
        seq_t base = (seq_t)(4 * k);
        seq_t exp  = (seq_t)(4 * k - 1);
        return big_pow(base, exp);
    }
}
