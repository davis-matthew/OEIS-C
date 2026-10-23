/*
 * A021111  Decimal expansion of 1/107.
 * https://oeis.org/A021111
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, cons, easy
 * Formula:   a(n) = floor( (10 * (10^n mod 107)) / 107 ) – decimal digit of
 *            1/107
 * Range:     u8 n <= 98, u16 n <= 98, u32 n <= 98, u64 n <= 98, u128 n <= 98,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

seq_t a(idx_t n)
{
    /* Compute 10^n mod 107 using binary exponentiation */
    uint64_t rem = 1;          /* current value of 10^k mod 107  */
    uint64_t base = 10ULL % 107ULL;   /* base = 10 mod 107 */

    for (idx_t e = n; e > 0; e >>= 1) {
        if (e & 1ULL) {
            rem = (rem * base) % 107ULL;
        }
        base = (base * base) % 107ULL;
    }

    /* The n-th decimal digit is floor((rem * 10) / 107) */
    seq_t digit = (seq_t)((rem * 10ULL) / 107ULL);
    return digit;
}
