/*
 * A039724  a(n) is the negabinary expansion of n, that is, the expansion of n
 *          in base -2.
 * https://oeis.org/A039724
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  base, nice, nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 5, u16 n <= 21, u32 n <= 341, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Negabinary expansion of n – returns a decimal integer whose
   decimal digits equal the base –2 representation of n. */

#include <stddef.h>

seq_t a(idx_t n)
{
    /* Zero is a special, trivial case. */
    if (n == 0)
        return 0;

    /* Store least‑significant negabinary digits first. */
    unsigned int digits[128];
    size_t pos = 0;

    /* Use a signed 128‑bit integer to avoid overflow while
       converting. This type is available on GCC/Clang. */
    signed __int128 x = n;

    while (x != 0) {
        /* The current remainder in base –2 is 0 or 1.  The
           mask x & 1 works for positive and negative values. */
        unsigned int r = (unsigned int)(x & 1);
        digits[pos++] = r;

        /* Prepare the next value: n = (r - n) / 2  →  x = (r - x) / 2. */
        x = (r - x) / 2;
    }

    /* Build the decimal integer from most‑significant to least‑significant
       digit.  Overflow is allowed – it simply wraps in an unsigned type
       as required. */
    seq_t result = 0;
    for (size_t i = pos; i-- > 0; ) {
        result = result * 10 + (seq_t)digits[i];
    }
    return result;
}
