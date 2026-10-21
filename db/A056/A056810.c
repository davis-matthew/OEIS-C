/*
 * A056810  Numbers whose fourth power is a palindrome.
 * https://oeis.org/A056810
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base, more
 * Formula:   a(1) = 0, a(2) = 1, a(n) = 10^(n-2) + 1 for n >= 3
 * Range:     u8 n <= 4, u16 n <= 6, u32 n <= 11, u64 n <= 18, u128 n <= 18,
 *            big any n
 * Helpers:   pow10
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computes a(n) : Numbers whose fourth power is a palindrome (A056810). */
#include "../utils/pow10.h"   /* provides seq_t pow10(idx_t); */

seq_t a(idx_t n)
{
    if (n == 1) return (seq_t)0;   /* a(1) = 0 */
    if (n == 2) return (seq_t)1;   /* a(2) = 1 */
    /* for n >= 3: a(n) = 10^(n-2) + 1 */
    seq_t p = pow10(n - 2);        /* compute 10^(n-2) */
    return p + (seq_t)1;
}
