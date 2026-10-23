/*
 * A119959  p^2-p+1 central polygonal numbers with prime indices
 *          A002061(prime(n)).
 * https://oeis.org/A119959
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = prime(n)^2 - prime(n) + 1
 * Range:     u8 n <= 6, u16 n <= 41, u32 n <= 41, u64 n <= 41, u128 n <= 41,
 *            big any n
 * Helpers:   get_prime
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include "../utils/get_prime.h"

seq_t a(idx_t n)
{
    if (n < 1) return 0;
    seq_t p = (seq_t)get_prime(n);   /* nth prime; 1‑indexed */
    return p * p - p + 1;
}
