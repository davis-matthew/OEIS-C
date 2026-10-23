/*
 * A012132  Numbers z such that x*(x+1) + y*(y+1) = z*(z+1) is solvable in
 *          positive integers x,y.
 * https://oeis.org/A012132
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) is the nth natural number z such that 2*z*(z+1)+1 is
 *            composite
 * Range:     u8 n <= 5, u16 n <= 188, u32 n <= 188, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   is_prime_big
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdbool.h>
#include "../utils/is_prime_big.h"

seq_t a(idx_t n)
{
    if (n == 0) abort();   /* sequence defined for n ≥ 1 only */

    idx_t count = 0;

    for (idx_t z = 1; ; ++z) {
        /* compute val = 2*z*(z+1)+1 using seq_t arithmetic only */
        seq_t val = (seq_t)2 * (seq_t)z * ((seq_t)z + (seq_t)1) + (seq_t)1;
        if (!is_prime_big(val)) {
            ++count;
            if (count == n) return (seq_t)z;
        }
    }
}
