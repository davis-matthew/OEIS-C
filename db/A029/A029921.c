/*
 * A029921  Convert n from miles to kilometers (km).
 * https://oeis.org/A029921
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = floor(n*25146/15625 + 1/2)
 * Range:     u8 n <= 0, u16 n <= 2, u32 n <= 55, u64 n <= 55, u128 n <= 55,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

seq_t a(idx_t n)
{
    /* constants chosen so that the operations stay within the width of seq_t */
    const seq_t mul   = (seq_t)25146;    /* multiplier in the numerator */
    const seq_t den   = (seq_t)15625;    /* denominator                     */
    const seq_t half  = den >> 1;        /* floor(den/2)  = 7812             */

    seq_t t = (seq_t)n * mul;            /* n * 25146 */
    return (t + half) / den;            /* rounded division */
}
