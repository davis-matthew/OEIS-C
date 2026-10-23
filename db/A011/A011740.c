/*
 * A011740  A binary m-sequence: expansion of reciprocal of x^27 + x^8 + x^7 +
 *          x + 1 (mod 2, shifted by 26 initial 0's).
 * https://oeis.org/A011740
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 80, u16 n <= 80, u32 n <= 80, u64 n <= 80, u128 n <= 80,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Recurrence:  a(0..25) = 0,  a(26) = 1,
   a(n) = a(n-1) ^ a(n-7) ^ a(n-8) ^ a(n-27)  for n ≥ 27
   (coefficients computed modulo 2).
   This implements the binary m‑sequence given by the reciprocal of
   x^27 + x^8 + x^7 + x + 1 (GF(2)), shifted by 26 initial 0's. */
#include <stdlib.h>
#include <stdint.h>

seq_t a(idx_t n)
{
    /* The sequence repeats with period 2^27 – 1 = 134,217,727. */
    const idx_t PERIOD = ((idx_t)1 << 27) - 1;

    /* Reduce the index modulo the period.  For any integer n the
       recurrence is periodic with the above period. */
    idx_t idx = n % PERIOD;

    /* For indices below 26 the terms are 0; a(26) = 1. */
    if (idx < 26) return (seq_t)0;
    if (idx == 26) return (seq_t)1;

    /* Use a circular buffer of the last 27 values to generate
       the sequence iteratively.  Only 27 slots are needed even
       when idx is large, so memory usage remains tiny. */
    seq_t buf[27] = {0};
    size_t pos = 26;          /* buf[pos] holds a(current index) */
    buf[pos] = (seq_t)1;      /* a(26) = 1 */

    for (idx_t k = 27; k <= idx; ++k) {
        /* Compute a(k) = a(k-1) ^ a(k-7) ^ a(k-8) ^ a(k-27) */
        seq_t next =
            buf[pos]                           /* a(k-1)   */
            ^ buf[(pos + 27 - 6) % 27]         /* a(k-7)   */
            ^ buf[(pos + 27 - 7) % 27]         /* a(k-8)   */
            ^ buf[(pos + 27 - 26) % 27];       /* a(k-27)  */

        pos = (pos + 1) % 27;  /* advance to next slot (newest term) */
        buf[pos] = next;
    }

    return buf[pos];
}
