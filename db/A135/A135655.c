/*
 * A135655  Divisors of 33550336 (the 5th perfect number), written in base 2.
 * https://oeis.org/A135655
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  base, nonn, fini, full, easy, less
 * Formula:   a(n) = (n <= 13) ? 10^(n-1) : ((10^13 - 1) / 9) * 10^(n-14)
 * Range:     u8 n <= 3, u16 n <= 5, u32 n <= 10, u64 n <= 21, u128 n <= 26,
 *            big any n
 * Helpers:   pow10
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Uses pow10 from ../utils/pow10.h */
#include <stdbool.h>
#include "../utils/pow10.h"

seq_t a(idx_t n)
{
    if (n < 1)
        return 0;               /* index below offset; undefined, return 0 */

    if (n <= 13)
        return pow10(n - 1);    /* 1 followed by (n-1) zeros */

    /* n >= 14 : 13 ones followed by (n-14) zeros */
    seq_t ones13 = (pow10(13) - 1) / 9;   /* 1111111111111 */
    return ones13 * pow10(n - 14);
}
