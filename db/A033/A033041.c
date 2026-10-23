/*
 * A033041  Numbers all of whose base 16 digits are odd.
 * https://oeis.org/A033041
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   not stated
 * Range:     u8 n <= 55, u16 n <= 55, u32 n <= 55, u64 n <= 55, u128 n <= 55,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* This function returns the n‑th term of A033041:
   Numbers all of whose base 16 digits are odd.
   It builds the number digit‑by‑digit using a base‑8 counting
   scheme: each hex digit is 1,3,5,7,9,B,D,F   (2*d+1). */

#include <stdint.h>
#include <stddef.h>

seq_t a(idx_t n)
{
    if (n == 0)           /* undefined for 0, but guard anyway */
        return 0;

    /* Find the width w of the hexadecimal representation */
    idx_t idx     = n - 1;          /* zero‑based index */
    idx_t block   = 8;              /* count of numbers with width 1 */
    idx_t sum     = 0;              /* cumulative count of smaller widths */
    size_t w      = 1;              /* current width */

    while (idx >= block) {
        sum += block;
        if (block > (UINT64_MAX / 8))
            break;                  /* avoid overflow for very large n */
        block *= 8;
        w++;
    }

    /* Offset within the current width block */
    idx_t offset = idx - sum;

    /* Extract base‑8 digits (least significant first) */
    uint8_t digits[32] = {0};       /* enough for n up to 8^31 */
    for (size_t i = 0; i < w; i++) {
        digits[i] = (uint8_t)(offset % 8);
        offset   /= 8;
    }

    /* Convert to hexadecimal number with odd digits */
    seq_t value = 0;
    for (size_t pos = 0; pos < w; pos++) {
        uint8_t d = digits[w - 1 - pos];   /* most significant digit first */
        seq_t mapped = (seq_t)(2 * d + 1);  /* 1,3,5,7,9,B,D,F */
        value = value * 16 + mapped;
    }

    return value;
}
