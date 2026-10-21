/*
 * A231631  Least positive integer k < n with k!*(n-k) + 1 prime, or 0 if such
 *          an integer k does not exist.
 * https://oeis.org/A231631
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 17, u16 n <= 59, u32 n <= 59, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   is_prime_big
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Compute a( n ) for OEIS A231631:
   least k < n with  k! * (n – k) + 1  prime, or 0 if none */
/* The function iterates over k = 1 .. n-1, updating k! in a seq_t
   and testing the resulting value with is_prime_big().

   Requires the helper is_prime_big from the utils library, which
   accepts a value of type seq_t. */
#include <stdbool.h>
#include "../utils/is_prime_big.h"

seq_t a(idx_t n)
{
    if (n <= 1)
        return (seq_t)0;                 /* no k < 1 */

    seq_t fact = (seq_t)1;              /* runs k! */
    for (idx_t k = 1; k < n; ++k)
    {
        fact *= (seq_t)k;                /* fact = k! for this k */
        seq_t val = fact * (seq_t)(n - k) + (seq_t)1;/* k!*(n-k)+1 */
        if (is_prime_big(val))
            return (seq_t)k;            /* first (smallest) k works */
    }
    return (seq_t)0;                     /* no suitable k found */
}
