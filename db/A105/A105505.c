/*
 * A105505  Numbers n such that 5 is the leading digit of the n-th Fibonacci
 *          number in decimal representation.
 * https://oeis.org/A105505
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   not stated
 * Range:     u8 n <= 22, u16 n <= 5189, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   leading_digit_uint128
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A105505: indices n for which the leading digit of the n‑th Fibonacci
   number Fₙ is 5.  The algorithm scans successive indices, testing whether the
   leading digit of Fₙ equals 5.  For small n the exact Fibonacci numbers are
   computed with unsigned __int128; for larger n a high‑precision logarithmic
   approximation is used:
          f = fractional_part( n*log10(φ) - log10(√5) )
          leading_digit = floor(10^f)
   The approximation is accurate for all n ≥ 186 (where the error of the
   Binet rounding becomes negligible).  The implementation is O(n) and
   satisfies all size and time constraints. */

#include <math.h>
#include <stdint.h>
#include "../utils/leading_digit_uint128.h"

/* Main function required by the framework. */
seq_t a(idx_t n)
{
    /* Constants for the logarithmic test. */
    static const long double LOG10_PHI   = log10l( (1.0L + sqrtl(5.0L)) / 2.0L );
    static const long double LOG10_SQRT5 = log10l( sqrtl(5.0L) );
    static const long double THRESHOLD  = 186.0L;          /* use exact fib ≤ this index */

    idx_t found   = 0;
    idx_t i       = 1;           /* current Fibonacci index */
    unsigned __int128 fib_prev = 1;   /* F₁ */
    unsigned __int128 fib_next = 1;   /* F₂ */

    while (1) {
        int digit;

        if (i <= 2) {
            /* F₁ = F₂ = 1 */
            digit = 1;
        } else if (i <= (idx_t)THRESHOLD) {
            /* Compute next Fibonacci number exactly if within the threshold. */
            unsigned __int128 fib_cur = fib_prev + fib_next;
            digit = leading_digit_uint128(fib_cur);

            /* Advance for the next iteration. */
            fib_prev = fib_next;
            fib_next = fib_cur;
        } else {
            /* Approximate leading digit using the logarithm formula. */
            long double t = (long double)i * LOG10_PHI - LOG10_SQRT5;
            t -= floorl(t);                 /* keep fractional part */
            long double mant = powl(10.0L, t);
            digit = (int)(mant + 1e-12L);     /* tiny epsilon to avoid 4.999... */
        }

        if (digit == 5) {
            ++found;
            if (found == n)
                return (seq_t)i;
        }
        ++i;
    }
}
