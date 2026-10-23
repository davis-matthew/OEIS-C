/*
 * A180353  a(n) = n^n * prime(n).
 * https://oeis.org/A180353
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = n^n * prime(n)
 * Range:     u8 n <= 3, u16 n <= 5, u32 n <= 8, u64 n <= 14, u128 n <= 18,
 *            big any n
 * Helpers:   big_pow, get_prime
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/big_pow.h"
#include "../utils/get_prime.h"

seq_t a(idx_t n)
{
    if (n == 0) return 0;
    seq_t pow_val = big_pow((seq_t)n, (seq_t)n);   /* n^n */
    seq_t prime   = (seq_t)get_prime(n);           /* nth prime */
    return pow_val * prime;
}
