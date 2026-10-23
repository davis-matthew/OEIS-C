/*
 * A021248  Decimal expansion of 1/244.
 * https://oeis.org/A021248
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, cons, easy
 * Formula:   a(n) = nth decimal digit of 1/244 * The digit is obtained by
 *            long‑division: starting with remainder 1, * for each position i
 *            (0‑based) compute * remainder *= 10; * digit = remainder / 244;
 *            * remainder %= 244; * The digit after the decimal point at index
 *            n is the value of * `digit` after the nth i...
 * Range:     u8 n <= 98, u16 n <= 98, u32 n <= 98, u64 n <= 98, u128 n <= 98,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

seq_t a(idx_t n)
{
    const uint64_t denom = 244ULL;   /* denominator of the fraction 1/244 */
    uint64_t remainder = 1ULL;       /* start remainder = 1 (numerator) */
    seq_t digit = 0;                 /* last digit computed */

    for (idx_t i = 0; i <= n; ++i) {
        remainder *= 10ULL;          /* shift decimal point */
        digit = (seq_t)(remainder / denom); /* next digit */
        remainder %= denom;         /* new remainder */
    }

    return digit;
}
