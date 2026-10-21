/*
 * A067053  Floor[ Sum_{1..n} 1/i ]^n.
 * https://oeis.org/A067053
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = (floor(∑_{i=1}^{n} 1/i))^n
 * Range:     u8 n <= 7, u16 n <= 10, u32 n <= 20, u64 n <= 25, u128 n <= 25,
 *            big any n
 * Helpers:   big_pow
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include "../utils/big_pow.h"

seq_t a(idx_t n)
{
    /* 2^45 ≈ 3.5×10^13 gives ample precision for n ≤ 10000
       (10^4 / 2^45 < 10^-7), so truncating after each add
       does not affect the integer part.  */
    const unsigned __int128 SCALE = ((unsigned __int128)1) << 45;

    unsigned __int128 sum = 0;
    for (idx_t i = 1; i <= n; ++i)
        sum += SCALE / i;          /* integer division, truncates */

    seq_t base = (seq_t)(sum / SCALE);  /* floor of the harmonic sum */

    return big_pow(base, (seq_t)n);     /* base^n */
}
