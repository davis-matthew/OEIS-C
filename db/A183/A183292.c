/*
 * A183292  Complement of A055999.
 * https://oeis.org/A183292
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = n + m - 3 if 2*n <= (m-2)*(m+3) n + m - 2 otherwise where
 *            m = floor(sqrt(8*n+25)/2)
 * Range:     u8 n <= 80, u16 n <= 80, u32 n <= 80, u64 n <= 80, u128 n <= 80,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

static seq_t isqrt128(unsigned __int128 x) {
    unsigned __int128 r = 0;
    unsigned __int128 bit = (unsigned __int128)1 << 126;  /* highest power of 4 <= 2^128 */
    while (bit > x) bit >>= 2;
    while (bit != 0) {
        if (x >= r + bit) {
            x -= r + bit;
            r = (r >> 1) + bit;
        } else {
            r >>= 1;
        }
        bit >>= 2;
    }
    return (seq_t)r;
}

seq_t a(idx_t n) {
    /* Compute m = floor(sqrt(8*n+25)/2) */
    unsigned __int128 tmp = (unsigned __int128)8 * (unsigned __int128)n + 25;
    seq_t sqrt_val = isqrt128(tmp);
    seq_t m = sqrt_val >> 1;                         /* divide by 2 */

    seq_t lhs = (seq_t)2 * (seq_t)n;
    seq_t rhs = (seq_t)(m - 2) * (seq_t)(m + 3);

    if (lhs <= rhs)
        return (seq_t)n + m - 3;
    else
        return (seq_t)n + m - 2;
}
