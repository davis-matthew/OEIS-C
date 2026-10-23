/*
 * A040114  List of absolute values of differences between digits of 10, 11,
 *          12, ..., listed digit by digit.
 * https://oeis.org/A040114
 *
 * Offset:    10    (a(10) is the first term)
 * Keywords:  nonn, base, less
 * Formula:   not stated
 * Range:     u8 n <= 1902, u16 n <= 1902, u32 n <= 1902, u64 n <= 1902,
 *            u128 n <= 1902, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computes a(n) for OEIS A040114:
   Absolute differences between consecutive digits of the
   natural numbers starting with 10.  a(10) is the first term
   (difference in 10).  The function returns 0 for n < 10. */
#include <stddef.h>   /* size_t */

/* seq_t and idx_t are provided by the build system.  */
seq_t a(idx_t n)
{
    /* Undefined for n < 10 – return 0 (not used). */
    if (n < 10) return 0;

    /* k is the zero‑based position among all produced differences. */
    idx_t k = n - 10;
    idx_t m = 10;          /* current integer being processed */
    idx_t count = 0;       /* number of differences produced so far */

    /* Temporary buffer for digits of m.  20 digits suffice for
       64‑bit integers. */
    uint8_t digs[20];
    size_t len;

    while (1) {
        /* Extract the decimal digits of m into digs[0..len-1]
           where digs[0] is the least significant digit. */
        idx_t tmp = m;
        len = 0;
        while (tmp > 0) {
            digs[len++] = (uint8_t)(tmp % 10);
            tmp /= 10;
        }

        /* Number of differences contributed by m. */
        idx_t diffs = len - 1;
        if (k < count + diffs) {
            /* The desired difference lies within this integer. */
            idx_t offset = k - count;            /* 0‑based within this integer */
            /* Most significant digit corresponds to digs[len-1].  */
            uint8_t d1 = digs[len - 1 - offset];
            uint8_t d2 = digs[len - 2 - offset];
            seq_t diff = (d1 > d2) ? (seq_t)(d1 - d2) : (seq_t)(d2 - d1);
            return diff;
        }

        /* Advance to the next integer. */
        count += diffs;
        ++m;
    }
}
