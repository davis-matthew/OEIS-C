/*
 * A233345  Numbers having at least one digit 0 and at least two identical
 *          digits (which may be two 0's), in their decimal expansion.
 * https://oeis.org/A233345
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   not stated
 * Range:     u8 n <= 6, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  This function returns the n‑th integer (n ≥ 1) that
   contains at least one digit 0 and at least two equal digits
   (the equal digits may be 0).  It enumerates all positive
   integers in increasing order and counts those satisfying
   the property.  */
#include <stdbool.h>

seq_t a(idx_t n) {
    /* n ≤ 0 is outside the offset – return 0 for safety. */
    if (n == 0) return 0;

    seq_t x = 100;           /* first candidate */
    idx_t count = 1;         /* a(1) = 100 */

    if (n == 1) return x;

    while (1) {
        x++;                  /* next integer */
        seq_t y = x;
        bool has_zero = false;
        bool has_repeat = false;
        unsigned char freq[10] = {0};

        /* Scan decimal digits of y. */
        while (y > 0) {
            unsigned d = (unsigned)(y % 10);
            if (d == 0) has_zero = true;
            if (freq[d] == 1) has_repeat = true;
            freq[d] = (unsigned char)(freq[d] + 1);
            y /= 10;
        }

        if (has_zero && has_repeat) {
            count++;
            if (count == n) return x;
        }
    }
}
