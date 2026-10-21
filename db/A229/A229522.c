/*
 * A229522  Final digit (in decimal system) of (n^n)^n, i.e., (n^n)^n mod 10.
 * https://oeis.org/A229522
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base, easy
 * Formula:   a(n) = last decimal digit of (n^n)^n = n^(n^2) mod 10
 * Range:     u8 n <= 86, u16 n <= 86, u32 n <= 86, u64 n <= 86, u128 n <= 86,
 *            big any n
 * Helpers:   small_pow10
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computes the final digit of (n^n)^n for any n >= 1.
 * The sequence is periodic with period 10. */
#include <stdint.h>
#include "../utils/small_pow10.h"

seq_t a(idx_t n)
{
    /* n is guaranteed to be >= 1 by the sequence definition. */
    unsigned base = (unsigned)(n % 10ULL);
    unsigned digit;

    switch (base) {
    case 0: digit = 0; break;
    case 1: digit = 1; break;
    case 5: digit = 5; break;
    case 6: digit = 6; break;
    default:
        /* For bases coprime to 10, the power cycle length is 4.  */
        unsigned exp = (unsigned)((((n % 4ULL) * (n % 4ULL)) % 4ULL));
        if (exp == 0) exp = 4;           /* exponent divisible by 4 → use cycle length */
        digit = small_pow10(base, exp);
        break;
    }

    return (seq_t)digit;
}
