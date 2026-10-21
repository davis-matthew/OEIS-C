/*
 * A053313  a(n) contains n digits (either '2' or '9') and is divisible by
 *          2^n.
 * https://oeis.org/A053313
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  base, nonn
 * Formula:   a(n) = a(n-1) + 10^(n-1)*(2 + 7*[a(n-1)/2^(n-1) mod 2]), * with
 *            a(1) = 2. The sequence contains only the digits 2 and 9. * * The
 *            implementation iterates from the first term up to n, keeping *
 *            only the current value and the needed powers of 10 and 2. * All
 *            arithmetic is performed in the generic in...
 * Range:     u8 n <= 2, u16 n <= 4, u32 n <= 10, u64 n <= 19, u128 n <= 39,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>

seq_t a(idx_t n)
{
    if (n == 0) {
        /* Offset 1: a(0) is undefined; return 0 safely. */
        return 0;
    }

    seq_t value = 2;      /* a(1) */
    if (n == 1) {
        return value;
    }

    seq_t pow10 = 1;      /* 10^(i-1) for i=1 initially */
    seq_t pow2  = 2;      /* 2^i for i=1 initially */

    for (idx_t i = 2; i <= n; ++i) {
        /* Update the powers for the current i */
        pow10 *= 10;
        seq_t pow2_next = pow2 * 2;    /* 2^i */

        /* Determine if a(i-1) is divisible by 2^i */
        bool divisible = false;
        if (pow2_next != 0) {
            divisible = (value % pow2_next == 0);
        }

        /* Choose the leading digit: 2 if divisible, else 9 */
        seq_t digit = divisible ? 2 : 9;

        /* Append the digit at the front */
        value = value + pow10 * digit;

        /* Prepare pow2 for the next iteration */
        pow2 = pow2_next;
    }

    return value;
}
