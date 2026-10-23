/*
 * A136697  Final nonzero digit of n! in base 11.
 * https://oeis.org/A136697
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  base, easy, nonn
 * Formula:   a(n) = last non-zero digit of n! written in base 11 *
 *            (equivalently, the product of all integers from 1 to n with
 *            every * factor of 11 removed, taken modulo 11)
 * Range:     u8 n <= 94, u16 n <= 94, u32 n <= 94, u64 n <= 94, u128 n <= 94,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

/* Compute the final non‑zero digit of n! in base 11. */
seq_t a(idx_t n)
{
    seq_t r = 1;                     /* running product modulo 11 */
    for (idx_t k = 1; k <= n; ++k) { /* iterate over 1..n */
        idx_t t = k;
        /* strip all powers of 11 from t */
        while ((t % 11) == 0) {
            t /= 11;
        }
        /* multiply by the reduced factor (mod 11) */
        r = (r * (t % 11)) % 11;
    }
    return r;
}
