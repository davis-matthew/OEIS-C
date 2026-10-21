/*
 * A096945  Eighth column of (1,5)-Pascal triangle A096940.
 * https://oeis.org/A096945
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = (n+35)*binomial(n+6, 6)/7 (binomial(k,6) = C(k,6))
 * Range:     u8 n <= 2, u16 n <= 10, u32 n <= 33, u64 n <= 33, u128 n <= 33,
 *            big any n
 * Helpers:   binom_nk
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/binom_nk.h"

seq_t a(idx_t n)
{
    /* C(n+6,6) */
    seq_t c = binom_nk(n + 6, 6);

    /* (n+35)*C(n+6,6) / 7 */
    unsigned __int128 temp = (unsigned __int128)c * (n + 35);
    temp /= 7;

    return (seq_t)temp;
}
