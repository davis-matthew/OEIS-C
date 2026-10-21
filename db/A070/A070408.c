/*
 * A070408  a(n) = 7^n mod 22.
 * https://oeis.org/A070408
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 7^n mod 22
 * Range:     u8 n <= 1000, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

seq_t a(idx_t n)
{
    /* iterative binary exponentiation */
    seq_t result = 1;
    seq_t base   = 7;          /* 7 mod 22 is 7 */
    idx_t exp    = n;

    while (exp > 0) {
        if (exp & 1U) {
            result = (result * base) % 22;
        }
        base = (base * base) % 22;
        exp >>= 1;
    }
    return result;
}
