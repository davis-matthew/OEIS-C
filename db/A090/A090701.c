/*
 * A090701  a(n) is the minimal number k such that every binary word of length
 *          n can be divided into k palindromes.
 * https://oeis.org/A090701
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = floor(n/6) + floor((n+4)/6) + 1; with the single
 *            exception a(11) = 5 (the only value that does not follow the
 *            formula). n is 1‑based
 * Range:     u8 n <= 763, u16 n <= 9941, u32 n <= 9941, u64 n <= 9941,
 *            u128 n <= 9941, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
seq_t a(idx_t n)
{
    /* handle the lone exceptional case */
    if (n == 11)
        return (seq_t)5;

    /* compute floor(n/6) and n mod 6 */
    idx_t q = n / 6;          /* floor(n/6)   */
    idx_t r = n % 6;          /* n mod 6       */

    /* floor((n+4)/6) = q + floor((r+4)/6) */
    idx_t extra = (r + 4) / 6;   /* 0 or 1 */

    /* a(n) = q + (q + extra) + 1 = 2*q + extra + 1 */
    return (seq_t)(2 * q + extra + 1);
}
