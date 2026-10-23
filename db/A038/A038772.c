/*
 * A038772  Numbers not divisible by any of their digits.
 * https://oeis.org/A038772
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  base, easy, nonn, nice
 * Formula:   not stated
 * Range:     u8 n <= 42, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Compute the n‑th number whose decimal digits do not divide it
 * (OEIS A038772).  Digits equal to 0 are ignored – only digits 1‑9 are
 * tested.  1 and any other digit that divides the number exclude it. */
#include <stdbool.h>

seq_t a(idx_t n)
{
    /* It is convenient to start the search from 1, but we use a loop
       that stops as soon as the n‑th qualifying number is found. */
    seq_t candidate = 1;
    idx_t   count    = 0;
    seq_t   result   = 0;

    while (count < n) {
        seq_t x   = candidate;
        bool  ok  = true;

        /* Check each digit of x.  Digits 0 are ignored because division
           by zero is undefined and is not considered a disqualifying
           divisor.  Digit 1 always disqualifies because it divides every
           positive integer. */
        seq_t temp = x;
        while (temp > 0) {
            seq_t d = temp % (seq_t)10;
            temp /= (seq_t)10;
            if (d == 0)
                continue;          /* ignore zero digits */
            if (d == 1) {         /* any number is divisible by 1 */
                ok = false;
                break;
            }
            if (x % d == 0) {     /* divisible by this digit */
                ok = false;
                break;
            }
        }

        if (ok) {
            ++count;
            if (count == n) {
                result = candidate;
                break;
            }
        }
        ++candidate;
    }

    return result;
}
