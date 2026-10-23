/*
 * A103181  In decimal representation of n: replace all even digits with 0 and
 *          all odd digits with 1.
 * https://oeis.org/A103181
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, base, easy
 * Formula:   a(n) = Σ (d_k mod 2) * 10^k, where n = Σ d_k * 10^k (decimal
 *            digits)
 * Range:     u8 n <= 999, u16 n <= 9999, u32 n <= 9999, u64 n <= 9999,
 *            u128 n <= 9999, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* It replaces every even decimal digit with 0 and every odd digit with 1. */

#include <stdint.h>

seq_t a(idx_t n)
{
    seq_t res = 0;
    seq_t pow = 1;
    idx_t x = n;
    while (x > 0) {
        seq_t digit = x % 10;          /* current decimal digit */
        res += (digit & 1) * pow;      /* add 1*pow if digit is odd, else 0 */
        x /= 10;
        pow *= (seq_t)10;
    }
    return res;
}
