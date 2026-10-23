/*
 * A193493  Put the natural numbers together without spaces and read them five
 *          at a time advancing one space each time.
 * https://oeis.org/A193493
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   a(n) = the 5‑digit integer formed by the decimal digits at
 *            positions n .. n+4 of the infinite concatenation
 *            1234567891011121314…
 * Range:     u8 none, u16 n <= 5, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdint.h>
#include <stdlib.h>

/* Return the nth term of A193493.  n >= 1.  Zero or negative n
   returns 0 (undefined behaviour for such inputs).  OEIS offset is 1. */
seq_t a(idx_t n)
{
    if (n == 0)  /* undefined for n < 1 */
        return 0;

    /* We need the first n+4 digits (positions 1..n+4). */
    idx_t need = n + 4;

    /* Allocate buffer for the first need digits. */
    char *buf = (char *)malloc((size_t)need);
    if (!buf)
        return 0;                    /* allocation failure */

    idx_t pos = 0;                    /* current write position in buf */
    idx_t num = 1;                    /* next integer to concatenate */

    /* Build the concatenation until we have at least need digits. */
    while (pos < need) {
        /* Extract digits of num into a temporary buffer in reverse order. */
        unsigned char tmp[20];
        int len = 0;
        idx_t t = num;
        do {
            tmp[len++] = (unsigned char)('0' + (t % 10));
            t /= 10;
        } while (t > 0);

        /* Copy digits to buf in correct order, without exceeding need. */
        for (int i = len - 1; i >= 0 && pos < need; --i)
            buf[pos++] = (char)tmp[i];

        ++num;
    }

    /* Convert the 5 characters starting at position n-1 to a seq_t value. */
    seq_t val = 0;
    for (idx_t i = 0; i < 5; ++i)
        val = val * 10 + (seq_t)(buf[n - 1 + i] - '0');

    free(buf);
    return val;
}
