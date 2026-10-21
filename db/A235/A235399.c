/*
 * A235399  Numbers which are the digital sum of the cube of some prime.
 * https://oeis.org/A235399
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   a(n) = (9*(2*n-1) + 5*(-1)^n)/4 for n >= 3 a(1) = 8, a(2) = 9
 *            (initial values of the sequence)
 * Range:     u8 n <= 14, u16 n <= 57, u32 n <= 57, u64 n <= 57, u128 n <= 57,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdint.h>

seq_t a(idx_t n)
{
    /* Handle the first two terms explicitly. */
    if (n == 1) return (seq_t)8;
    if (n == 2) return (seq_t)9;

    /* For n >= 3, use the closed‑form formula. */
    seq_t two_n_minus_one = (seq_t)2 * (seq_t)n - (seq_t)1;          /* 2*n-1   */
    seq_t val = (seq_t)9 * two_n_minus_one;                         /* 9*(2*n-1) */

    /* Add or subtract 5 depending on the parity of n
       (-1)^n = 1 for even n, -1 for odd n. */
    if (n & 1)          /* n odd: subtract 5 */
        val -= (seq_t)5;
    else                /* n even: add 5 */
        val += (seq_t)5;

    return val / (seq_t)4;
}
