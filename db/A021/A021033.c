/*
 * A021033  Decimal expansion of 1/29.
 * https://oeis.org/A021033
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, cons
 * Formula:   a(n) = a(n-1) - a(n-14) + a(n-15). Initial values: a(0)=0,
 *            a(1)=3, a(2)=4, a(3)=4, a(4)=8, a(5)=2, a(6)=7, a(7)=5, a(8)=8,
 *            a(9)=6, a(10)=2, a(11)=0, a(12)=6, a(13)=8, a(14)=9, a(15)=6.
 *            The sequence is periodic with period 28 and coincides with the
 *            digits after the decimal point in 1/29
 * Range:     u8 n <= 98, u16 n <= 98, u32 n <= 98, u64 n <= 98, u128 n <= 98,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stddef.h>
 
seq_t a(idx_t n)
{
    /* Store one period of the repeating digit sequence.
       The period is 28 digits. The digits are computed once on
       first entry. */
    static seq_t digits[28];
    static int initialized = 0;
 
    if (!initialized) {
        uint64_t rem = 1;          /* start with remainder 1 (since 1/29) */
        for (size_t i = 0; i < 28; ++i) {
            uint64_t digit = (rem * 10) / 29;   /* integer part of next digit */
            digits[i] = (seq_t)digit;
            rem = (rem * 10) % 29;             /* new remainder for next digit */
        }
        initialized = 1;
    }
 
    /* Index modulo 28 to get the desired digit. */
    size_t idx = (size_t)(n % 28ULL);
    return digits[idx];
}
