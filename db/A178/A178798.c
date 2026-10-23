/*
 * A178798  Expansion of the polynomial
 *          x^36*(x^12-1)*(x^9+1)*(x^8-1)*(x^6-1)*(x^5-1)*(x^2-1) in
 *          increasing powers of x.
 * https://oeis.org/A178798
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign, fini, full
 * Formula:   a(n) = coefficient of x^n in *
 *            x^36*(x^12-1)*(x^9+1)*(x^8-1)*(x^6-1)*(x^5-1)*(x^2-1)
 * Range:     i8 n <= 78, i16 n <= 78, i32 n <= 78, i64 n <= 78, i128 n <= 78,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*   The sequence is zero for n < 36, reaches its last non‑zero term at n=78. */

#include <stddef.h>
#include <stdbool.h>

seq_t a(idx_t n)
{
    static seq_t coeff[79];
    static bool init = false;

    if (!init) {
        /* clear coefficients */
        for (int i = 0; i < 79; ++i)
            coeff[i] = 0;

        /* Constants for the six factors (x^k - 1) or (x^k + 1) */
        const seq_t consts[6] = { -1, +1, -1, -1, -1, -1 };

        /* Enumerate all 2^6 choices of taking the x^k term (value +1)
         * or the constant term (value consts[ ]). */
        for (int mask = 0; mask < (1 << 6); ++mask) {
            seq_t c = 1;      /* cumulative coefficient of this choice */
            int exp = 0;      /* sum of exponents of chosen x^k terms */

            /* factor (x^12-1) */
            if (mask & 1)       exp += 12;          /* take x^12 term */
            else               c *= consts[0];      /* take constant -1 */

            /* factor (x^9+1) */
            if (mask & 2)       exp += 9;
            else               c *= consts[1];

            /* factor (x^8-1) */
            if (mask & 4)       exp += 8;
            else               c *= consts[2];

            /* factor (x^6-1) */
            if (mask & 8)       exp += 6;
            else               c *= consts[3];

            /* factor (x^5-1) */
            if (mask & 16)      exp += 5;
            else               c *= consts[4];

            /* factor (x^2-1) */
            if (mask & 32)      exp += 2;
            else               c *= consts[5];

            /* shift by x^36 */
            int total_exp = exp + 36;

            coeff[(size_t)total_exp] += c;
        }

        init = true;
    }

    if (n < 79)
        return coeff[(size_t)n];
    return 0;
}
