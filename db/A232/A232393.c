/*
 * A232393  Michael Stifel's 4 X 4 multiplication magic square read by rows.
 * https://oeis.org/A232393
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, fini, full, tabf
 * Formula:   a(n) = * 2^p[(n-1) mod 16] * where p is the exponent order of
 *            the 4×4 multiplication magic square read by rows: * 15, 2, 1,
 *            12, 8, 5, 6, 11, 4, 9, 10, 7, 3, 14, 13, 0. * This implements
 *            the periodic 16‑term sequence derived from that square
 * Range:     u8 none, u16 n <= 16, u32 n <= 16, u64 n <= 16, u128 n <= 16,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>

seq_t a(idx_t n)
{
    if (n == 0) return 0;                /* undefined for n < 1 */
    static const unsigned char order[16] = {
        15, 2, 1, 12,
        8, 5, 6, 11,
        4, 9, 10, 7,
        3, 14, 13, 0
    };
    unsigned idx = (unsigned)((n - 1) % 16);
    return ((seq_t)1) << order[idx];
}
