/*
 * A120500  Times in hours,minutes and seconds (to the nearest second) at
 *          which the smoothly crossing minute and hour hands of an analog
 *          clock coincide, over a period of one complete 12-hour sweep of the
 *          hour hand.
 * https://oeis.org/A120500
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  fini, full, nonn
 * Formula:   a(n) = round(43200*n/11) expressed on a double‑spaced
 *            sexagesimal scale (concatenated hour, minute, second digits)
 * Range:     u8 n <= 0, u16 n <= 1, u32 n <= 11, u64 n <= 11, u128 n <= 11,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

seq_t a(idx_t n)
{
    /* Compute t = round(43200*n/11) without floating point. */
    seq_t prod = 43200 * (seq_t)n;     /* 43200 * n */
    seq_t div  = prod / 11;            /* floor(43200*n/11)   */
    seq_t rem  = prod % 11;            /* remainder of the division */
    if (rem >= 6)                      /* round up if remainder >= 5.5 */
        div += 1;
    seq_t t = div;                     /* exact integer time in seconds   */

    /* Convert seconds to hour, minute, second. */
    seq_t h = t / 3600;                /* hours   */
    seq_t m = (t % 3600) / 60;         /* minutes */
    seq_t s = t % 60;                  /* seconds */

    /* Concatenate as h*10000 + m*100 + s.  Hours are not padded, minutes and
       seconds are two‑digit zero padded implicitly by the multiplier 100. */
    return h * 10000 + m * 100 + s;
}
