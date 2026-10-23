/*
 * A028302  a(n) = prime(n)*Catalan(n).
 * https://oeis.org/A028302
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = prime(n) * Catalan(n)
 * Range:     u8 n <= 4, u16 n <= 8, u32 n <= 16, u64 n <= 32, u128 n <= 64,
 *            big any n
 * Helpers:   catalan, get_prime
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/get_prime.h"
#include "../utils/catalan.h"

seq_t a(idx_t n)
{
    if (n == 0)
        return 0;
    /* Retrieve the nth prime and the nth Catalan number. */
    seq_t prime = (seq_t)get_prime(n);
    seq_t cat   = catalan(n);
    return prime * cat;
}
