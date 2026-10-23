/*
 * A104686  a(n) = n*(n+1)/2 (mod 6).
 * https://oeis.org/A104686
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = n*(n+1)/2 (mod 6)
 * Range:     u8 n <= 86, u16 n <= 86, u32 n <= 86, u64 n <= 86, u128 n <= 86,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>

seq_t a(idx_t n)
{
    /* Triangular numbers modulo 6 repeat with period 12. */
    static const seq_t tbl[12] = {
        0, 1, 3, 0, 4, 3, 3, 4, 0, 3, 1, 0
    };
    return tbl[(size_t)(n % 12)];
}
