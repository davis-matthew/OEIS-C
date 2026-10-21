/*
 * A055115  Base-5 complement of n (write n in base 5, then replace each digit
 *          with its base-5 negative).
 * https://oeis.org/A055115
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  base, easy, nonn
 * Formula:   a(n) = Σ ((5 - d_i) % 5) * 5^i, where the d_i are the base‑5
 *            digits of n (least significant first)
 * Range:     u8 n <= 124, u16 n <= 3124, u32 n <= 3124, u64 n <= 3124,
 *            u128 n <= 3124, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

seq_t a(idx_t n)
{
    seq_t result = 0;
    seq_t power  = 1;
    idx_t  temp   = n;
    while (temp > 0) {
        idx_t digit = temp % 5;                   /* current base‑5 digit */
        idx_t comp  = (5 - digit) % 5;            /* its complement (5‑digit mod 5) */
        result += power * (seq_t)comp;
        power  *= (seq_t)5;
        temp   /= 5;
    }
    return result;
}
