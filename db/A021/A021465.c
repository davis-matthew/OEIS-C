/*
 * A021465  Decimal expansion of 1/461.
 * https://oeis.org/A021465
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

/* Decimal digit of 1/461: a(n) = the n-th digit after the decimal point in the expansion of 1/461. */
#include <stdint.h>
#include <stdbool.h>

seq_t a(idx_t n)
{
    /* static variables to cache the periodic digits of the decimal expansion */
    static seq_t digits[461];
    static idx_t period = 0;
    static bool initialized = false;

    /* one-time initialization: find period and compute all digits */
    if (!initialized) {
        /* Find the multiplicative order of 10 modulo 461 */
        idx_t rem = 10 % 461;        /* 10^1 mod 461 */
        idx_t k = 1;
        while (rem != 1) {
            rem = (rem * 10) % 461;
            k++;
        }
        period = k;                  /* the period length   */

        /* Compute all digits of the repeating block */
        rem = 1;                     /* start remainder is 1 */
        for (idx_t i = 0; i < period; ++i) {
            rem *= 10;
            digits[i] = rem / 461;   /* integer quotient = digit   */
            rem %= 461;              /* new remainder for next step */
        }

        initialized = true;
    }

    /* Return the required digit, using periodicity */
    return digits[n % period];
}
