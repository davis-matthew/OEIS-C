/*
 * A066323  Number of one bits in binary representation of base i-1 expansion
 *          of n (where i = sqrt(-1)).
 * https://oeis.org/A066323
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy, base
 * Formula:   not stated
 * Range:     u8 n <= 9999, u16 n <= 9999, u32 n <= 9999, u64 n <= 9999,
 *            u128 n <= 9999, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*
 * Formula: a(n) = sum of base -4 digits of n, where digits are in {0,1,2,3}.
 *
 * This implements A066323: number of one-bits in the binary representation
 * of the base (i-1) expansion of n.
 */
#include <stdint.h>

/* idx_t and seq_t are defined in the build */
seq_t a(idx_t n)
{
    /* use a signed 128‑bit integer to avoid overflow when n is large */
    __int128 sh = n;          /* signed representation of n */
    seq_t sum = 0;

    if (sh == 0)
        return sum;

    while (sh != 0) {
        /* remainder of division by -4; can be negative */
        int r = (int)(sh % (-4));
        sh = sh / (-4);            /* quotient */
        if (r < 0) {               /* adjust to get 0–3 */
            r += 4;
            sh += 1;
        }
        sum += (seq_t)r;
    }
    return sum;
}
