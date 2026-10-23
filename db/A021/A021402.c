/*
 * A021402  Decimal expansion of 1/398.
 * https://oeis.org/A021402
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, cons, easy
 * Formula:   not stated
 * Range:     u8 n <= 98, u16 n <= 98, u32 n <= 98, u64 n <= 98, u128 n <= 98,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* a(n) = floor( (10^n mod 398) * 10 / 398 )
   Computes the nth decimal digit after the point of 1/398. */
#include <stdint.h>

seq_t a(idx_t n)
{
    /* Compute r = 10^n mod 398 using binary exponentiation. */
    uint64_t r = 1;
    uint64_t base = 10 % 398;
    idx_t exp = n;
    while (exp) {
        if (exp & 1ULL)
            r = (r * base) % 398ULL;
        base = (base * base) % 398ULL;
        exp >>= 1;
    }

    /* The digit is floor(r * 10 / 398). */
    seq_t digit = (seq_t)((r * 10ULL) / 398ULL);
    return digit;
}
