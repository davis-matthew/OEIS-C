/*
 * A057901  a(n) = 3^prime(n).
 * https://oeis.org/A057901
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 3^prime(n)
 * Range:     u8 n <= 3, u16 n <= 4, u32 n <= 8, u64 n <= 12, u128 n <= 22,
 *            big any n
 * Helpers:   get_prime, pow3
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/get_prime.h"
#include "../utils/pow3.h"

seq_t a(idx_t n)
{
    if (n == 0)
        return 0;                 /* offset is 1; return 0 for safety */
    uint32_t prime = get_prime(n); /* prime(1)=2, prime(2)=3, ... */
    return pow3((idx_t)prime);     /* compute 3^prime and return the result */
}
