/*
 * A086067  Bit string encoding occurrence of digits of n in decimal
 *          representation: d-th bit is set iff d occurs in (n)10, 0 <= d <
 *          10.
 * https://oeis.org/A086067
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, base
 * Formula:   a(n) = decimal representation of the binary mask of decimal
 *            digits appearing in n
 * Range:     u8 n <= 2, u16 n <= 4, u32 n <= 29, u64 n <= 29, u128 n <= 29,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* The mask has bit d set iff the digit d occurs in the decimal representation of n (0 ≤ d < 10) */
/* a(n) is the integer whose decimal digits are the bits of that mask, from most‑significant to least. */

#include <stdint.h>

seq_t a(idx_t n)
{
    /* Build a 10‑bit mask of digits present in n. */
    unsigned int mask = 0;
    /* Handle n = 0 specially to include the digit 0. */
    do {
        unsigned int d = n % 10;
        mask |= 1u << d;
        n /= 10;
    } while (n > 0);

    /* Find the most significant set bit (0‑based). */
    int msb = -1;
    for (int i = 9; i >= 0; --i) {
        if (mask & (1u << i)) {
            msb = i;
            break;
        }
    }

    /* Construct the decimal number from the binary bits. */
    seq_t result = 0;
    for (int i = msb; i >= 0; --i) {
        result = result * 10 + ((mask >> i) & 1u);
    }
    return result;
}
