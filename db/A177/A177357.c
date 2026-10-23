/*
 * A177357  Number of positive squares <= prime(n) - 3.
 * https://oeis.org/A177357
 *
 * Offset:    2    (a(2) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = floor(sqrt(prime(n)-3))
 * Range:     u8 n <= 18, u16 n <= 79, u32 n <= 79, u64 n <= 79, u128 n <= 79,
 *            big any n
 * Helpers:   floor_sqrt, get_prime
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/get_prime.h"
#include "../utils/floor_sqrt.h"

seq_t a(idx_t n)
{
    if (n < 2) return 0;                 /* sequence offset is 2 */

    seq_t p = (seq_t)get_prime(n);       /* p = prime(n) */
    if (p < 3) return 0;                 /* safety for n=1 */

    seq_t x = p - (seq_t)3;              /* x = prime(n)-3 */
    return floor_sqrt(x);                /* count of positive squares <= x */
}
