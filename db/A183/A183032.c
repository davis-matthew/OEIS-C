/*
 * A183032  Seconds (rounded down) at which the minute hand overlaps with hour
 *          hand on the analog clock.
 * https://oeis.org/A183032
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, fini, full, easy
 * Formula:   a(n) = floor(300*n/11) (mod 60)
 * Range:     u8 n <= 0, u16 n <= 10, u32 n <= 10, u64 n <= 10, u128 n <= 10,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

seq_t a(idx_t n)
{
    /* Compute floor(300*n/11) and take the result modulo 60.
       All arithmetic is performed using the target type seq_t,
       which is guaranteed to be an unsigned integer type large enough
       for the intermediate value. */
    seq_t prod = (seq_t)n * (seq_t)300;        /* 300*n */
    seq_t div  = prod / (seq_t)11;            /* floor division */
    return div % (seq_t)60;                   /* modulo 60 */
}
