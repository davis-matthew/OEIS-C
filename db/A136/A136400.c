/*
 * A136400  Replace all digits greater than 1 with 1 (in decimal
 *          representation).
 * https://oeis.org/A136400
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, base, easy
 * Formula:   a(n) = replace all decimal digits > 1 with 1
 * Range:     u8 n <= 999, u16 n <= 9999, u32 n <= 9999, u64 n <= 9999,
 *            u128 n <= 9999, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>  /* for uint64_t constant */

seq_t a(idx_t n)
{
    seq_t result = 0;
    seq_t base   = 1;

    while (n > 0)
    {
        uint64_t digit = n % 10ULL;                /* current least‑significant decimal digit */
        seq_t    d2    = (digit > 1ULL) ? 1 : digit;/* 1 if digit >1, otherwise the digit itself */
        result += d2 * base;
        base   *= 10;
        n      /= 10;
    }
    return result;
}
