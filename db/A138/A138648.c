/*
 * A138648  7^n mod 5^n.
 * https://oeis.org/A138648
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 7^n mod 5^n (with a(0)=0)
 * Range:     u8 n <= 3, u16 n <= 6, u32 n <= 13, u64 n <= 23, u128 n <= 23,
 *            big any n
 * Helpers:   big_pow, mod_pow
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/big_pow.h"
#include "../utils/mod_pow.h"

seq_t a(idx_t n)
{
    if (n == 0)
        return (seq_t)0;
    seq_t mod = big_pow((seq_t)5, (seq_t)n);   /* compute 5^n */
    return mod_pow((seq_t)7, n, mod);           /* compute 7^n mod 5^n */
}
