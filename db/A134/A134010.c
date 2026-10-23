/*
 * A134010  a(n) = n^(initial digit of n).
 * https://oeis.org/A134010
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, base, look
 * Formula:   a(n) = n^A000030(n) (n raised to its initial decimal digit)
 * Range:     u8 n <= 3, u16 n <= 6, u32 n <= 59, u64 n <= 699,
 *            u128 n <= 10000, big any n
 * Helpers:   big_pow
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/big_pow.h"

static inline int first_digit(idx_t n)
{
    /* Return the first decimal digit of n.  For n == 0, returns 0. */
    if (n == 0)
        return 0;
    while (n >= 10)
        n /= 10;
    return (int)n;
}

seq_t a(idx_t n)
{
    if (n == 0)
        return (seq_t)1;                /* a(0) = 1 by definition */

    int d = first_digit(n);             /* initial digit of n (1..9) */
    seq_t base = (seq_t)n;
    seq_t exp  = (seq_t)d;
    return big_pow(base, exp);          /* compute n^d */
}
