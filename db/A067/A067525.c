/*
 * A067525  Define I(n) = number obtained by incrementing each digit from 0 to
 *          8 of n by 1. A '9' is replaced by a '0'. Sequence gives
 *          digitriangular numbers n*I(n)/2.
 * https://oeis.org/A067525
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  base, easy, nonn
 * Formula:   a(n) = n * I(n) / 2, where I(n) is the number obtained * by
 *            incrementing each decimal digit of n modulo 10 * (0→1, 1→2, …,
 *            8→9, 9→0). The function works for any idx_t * (unsigned 64‑bit)
 *            and any seq_t (unsigned integer or arbitrary‑precision)
 * Range:     u8 n <= 17, u16 n <= 51, u32 n <= 51, u64 n <= 51, u128 n <= 51,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

seq_t a(idx_t n)
{
    /* convert index to seq_t */
    const seq_t seq_n = (seq_t)n;

    /* compute I(n): incremental mapping of decimal digits */
    seq_t I  = 0;          /* result for I(n) */
    seq_t mult = 1;        /* 10^position factor */
    idx_t  tmp = n;        /* temporary variable for digit extraction */

    while (tmp > 0) {
        int d      = tmp % 10;          /* current decimal digit */
        int inc    = (d + 1) % 10;      /* incremented digit modulo 10 */
        I += (seq_t)inc * mult;         /* add digit contribution */
        mult *= 10;                      /* move to next position */
        tmp  /= 10;
    }
    /* for n == 0 the loop is skipped and I stays 0 */

    /* compute a(n) = n * I(n) / 2 */
    return (seq_n * I) / (seq_t)2;
}
