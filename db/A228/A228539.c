/*
 * A228539  Rows of binary Walsh matrices interpreted as reverse binary
 *          numbers.
 * https://oeis.org/A228539
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, tabf
 * Formula:   a(n) = value of row k of binary Walsh matrix of size 2^n,
 *            interpreted as a reverse binary number, where n =
 *            floor(log2(n+1)), k = n - (2^n - 1)
 * Range:     u8 n <= 15, u16 n <= 31, u32 n <= 63, u64 n <= 127,
 *            u128 n <= 255, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

seq_t a(idx_t n)
{
    /* Determine the order of the Walsh matrix: floor(log2(n+1)) */
    idx_t n_order = 0;
    idx_t tmp = n + 1;
    while (tmp >>= 1)
        ++n_order;

    /* Index of the first row of this order */
    idx_t start = ((idx_t)1 << n_order) - 1;
    idx_t k = n - start;           /* row index within this order   */
    idx_t size = ((idx_t)1 << n_order);   /* number of columns = 2^n_order */

    seq_t value   = 0;
    seq_t bitval  = 1;             /* 2^j, j starts at 0 */

    for (idx_t j = 0; j < size; ++j) {
        idx_t parity = __builtin_popcountll((unsigned long long)(k & j));
        if (parity & 1ULL)
            value += bitval;
        bitval += bitval;          /* multiply by 2 */
    }

    return value;
}
