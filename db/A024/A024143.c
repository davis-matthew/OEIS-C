/*
 * A024143  a(n) = 12^n - n^3.
 * https://oeis.org/A024143
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 12^n - n^3
 * Range:     u8 n <= 2, u16 n <= 4, u32 n <= 8, u64 n <= 17, u128 n <= 35,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>

seq_t a(idx_t n)
{
    /* Compute 12^n via binary exponentiation */
    seq_t result = 1;
    seq_t base   = 12;
    idx_t exp    = n;

    while (exp != 0) {
        if (exp & 1u) {
            result = result * base;
        }
        base = base * base;
        exp >>= 1;
    }

    /* Compute n^3 */
    seq_t nn  = (seq_t)n;
    seq_t nn2 = nn * nn;
    seq_t nn3 = nn2 * nn;

    /* Return 12^n - n^3 */
    result = result - nn3;
    return result;
}
