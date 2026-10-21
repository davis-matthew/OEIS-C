/*
 * A090115  a(n)=Product[p(n)-j, j=1..n]/n!=A090114(n)/n!.
 * https://oeis.org/A090115
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = binom(p(n)-1, n) (where p(n) is the n-th prime)
 * Range:     u8 n <= 4, u16 n <= 6, u32 n <= 11, u64 n <= 20, u128 n <= 20,
 *            big any n
 * Helpers:   binom, get_prime
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include "../utils/get_prime.h"
#include "../utils/binom.h"

seq_t a(idx_t n)
{
    /* p_n = the n-th prime */
    uint32_t prime_n = get_prime(n);
    idx_t   a = (idx_t)prime_n - (idx_t)1;
    /* a(n) = C(p(n)-1, n) */
    return binom(a, n);
}
