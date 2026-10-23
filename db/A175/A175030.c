/*
 * A175030  a(n)=3*n if n is prime, otherwise a(n)=2*n.
 * https://oeis.org/A175030
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 3*n if n is prime, otherwise 2*n
 * Range:     u8 n <= 65, u16 n <= 65, u32 n <= 65, u64 n <= 65, u128 n <= 65,
 *            big any n
 * Helpers:   is_prime
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include "../utils/is_prime.h"

seq_t a(idx_t n)
{
    if (n < 1)                     /* undefined for n < 1, return 0 */
        return (seq_t)0;
    if (is_prime(n))
        return (seq_t)n * (seq_t)3;
    else
        return (seq_t)n * (seq_t)2;
}
