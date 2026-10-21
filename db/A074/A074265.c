/*
 * A074265  Values of Kolakoski sequence A000002 at positions n = 0 mod 5.
 * https://oeis.org/A074265
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 10000, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computes the Kolakoski sequence A000002 at index 5*n. */
#include <stddef.h>
#include <stdlib.h>

/* seq_t and idx_t are defined by the build system. */
seq_t a(idx_t n)
{
    if (n == 0)
        return (seq_t)0;               /* offset starts at 1 */

    /* Number of terms of the Kolakoski sequence that must be generated. */
    idx_t needed = n * 5;

    /* Allocate a buffer large enough for the first `needed` terms. */
    seq_t *buf = (seq_t *)malloc((size_t)needed * sizeof(seq_t));
    if (!buf)
        return (seq_t)0;               /* allocation failure */

    /* Base cases. */
    if (needed >= 1)
        buf[0] = (seq_t)1;
    if (needed >= 2)
        buf[1] = (seq_t)2;

    /* Build the rest of the sequence when more than two terms are requested. */
    if (needed > 2) {
        size_t i = 2;                  /* next position to write (0‑based) */
        seq_t val = (seq_t)2;          /* value to write in the current run */
        size_t j = 1;                  /* index of the next run length in buf */
        size_t remaining = (size_t)buf[1] - 1;  /* copies left in the current run */

        while (i < (size_t)needed) {
            buf[i++] = val;            /* write the current value */
            --remaining;               /* one copy consumed */
            if (remaining == 0) {      /* run finished – start a new one */
                ++j;
                val = (seq_t)3 - val;   /* toggle between 1 and 2 */
                remaining = (size_t)buf[j];
            }
        }
    }

    seq_t result = buf[(size_t)needed - 1];
    free(buf);
    return result;
}
