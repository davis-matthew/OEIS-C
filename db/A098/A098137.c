/*
 * A098137  a(n) = n^n + n^prime(n).
 * https://oeis.org/A098137
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = n^n + n^prime(n)
 * Range:     u8 n <= 2, u16 n <= 4, u32 n <= 5, u64 n <= 8, u128 n <= 11,
 *            big any n
 * Helpers:   big_pow, get_prime
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/big_pow.h"
#include "../utils/get_prime.h"

seq_t a(idx_t n)
{
    if (n < 1)
        return (seq_t)0;          /* sequence is 1‑based; undefined for n=0 */

    seq_t base = (seq_t)n;           /* n as seq_t */

    /* compute n^n */
    seq_t term1 = big_pow(base, base);

    /* compute n^prime(n) */
    seq_t term2 = big_pow(base, (seq_t)get_prime(n));

    return term1 + term2;
}
