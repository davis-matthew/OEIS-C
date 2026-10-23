/*
 * A180263  Odd k such that (k^2 + 1)/2 is not prime.
 * https://oeis.org/A180263
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 6, u16 n <= 167, u32 n <= 167, u64 n <= 2000,
 *            u128 n <= 2000, big any n
 * Helpers:   is_prime_big
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Compute the sequence of odd integers k such that (k*k + 1)/2 is not prime.
   The sequence lists these k in increasing order, with offset 1:
   a(1) = 1, a(2) = 7, a(3) = 13, ... */
#include <stdbool.h>
#include "../utils/is_prime_big.h"

seq_t a(idx_t n)
{
    if (n == 0)               /* offset is 1, so index 0 is undefined */
        return (seq_t)0;

    seq_t k = 1;              /* start with the first odd integer */
    idx_t count = 0;          /* how many qualifying k we have found */

    while (1) {
        /* (k*k + 1)/2 */
        seq_t val = ((seq_t)k * (seq_t)k + 1) / 2;

        if (!is_prime_big(val)) {   /* val is composite or 1 */
            ++count;
            if (count == n)
                return k;
        }

        k += 2;   /* next odd integer */
    }
}
