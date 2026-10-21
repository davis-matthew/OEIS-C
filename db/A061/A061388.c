/*
 * A061388  Sum of digits = 5 times number of digits.
 * https://oeis.org/A061388
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base, easy
 * Formula:   a(n) = nth positive integer whose decimal digit sum equals 5 ×
 *            (number of digits)
 * Range:     u8 n <= 16, u16 n <= 3679, u32 n <= 9409, u64 n <= 9409,
 *            u128 n <= 9409, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* We simply search integers in increasing order and keep the ones that satisfy
   the condition.  For n up to about 10 000 this linear search is easily fast
   enough (≈ a few million digit operations).  The code uses only the integer
   operators allowed by the requirements. */

#include <stdint.h>

seq_t a(idx_t n)
{
    if (n < 1) return 0;

    uint64_t target = n;      /* we work with a 64‑bit counter for simplicity   */
    uint64_t count  = 0;      /* number of qualifying terms found so far        */
    uint64_t i      = 1;      /* integer to test                              */

    for (;; ++i)
    {
        uint64_t tmp   = i;
        uint64_t sum   = 0;
        int     digits = 0;

        /* compute digit sum and digit count */
        while (tmp > 0) {
            sum   += tmp % 10;
            digits++;
            tmp   /= 10;
        }

        /* test the OEIS condition */
        if (sum == 5ull * digits) {
            ++count;
            if (count == target)
                return (seq_t)i;
        }
    }
}
