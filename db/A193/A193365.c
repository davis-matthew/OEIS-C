/*
 * A193365  a(n) = A220371(n)/(4*A220371(n-1)).
 * https://oeis.org/A193365
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 2^p * (2^(2p+4) * m^2 - 1), where n = 2^p * m and m is
 *            odd (m = 2k - 1)
 * Range:     u8 n <= 3, u16 n <= 23, u32 n <= 34, u64 n <= 34, u128 n <= 34,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

seq_t a(idx_t n)
{
    if (n == 0) return 0;          /* Undefined for n=0, return 0 */
    /* Compute trailing zeros p and odd factor m */
    idx_t temp = n;
    idx_t p = 0;
    while ((temp & 1ULL) == 0) {
        ++p;
        temp >>= 1;
    }
    seq_t m = (seq_t)temp;                     /* odd */
    seq_t T = (seq_t)1 << (2 * p + 4);          /* 2^(2p+4) */
    seq_t term = T * m * m;                     /* 2^(2p+4) * m^2 */
    seq_t res = ((seq_t)1 << p) * (term - 1);   /* 2^p * (term - 1) */
    return res;
}
