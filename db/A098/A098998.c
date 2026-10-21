/*
 * A098998  p(11p-7) where p is prime.
 * https://oeis.org/A098998
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 3, u16 n <= 21, u32 n <= 60, u64 n <= 60, u128 n <= 60,
 *            big any n
 * Helpers:   nth_prime
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*
 * Formula: a(n) = p_n * (11 * p_n - 7) where p_n is the nth prime
 * (OEIS A098998, offset 1).
 */

#include <stdlib.h>
#include <stddef.h>
#include "../utils/nth_prime.h"

/* Compute A098998(n).  n is 1‑based; terms for n==0 are undefined. */
seq_t a(idx_t n)
{
    if (n == (idx_t)0) return 0;              /* offset guard */

    idx_t p = nth_prime(n);                   /* n-th prime */
    seq_t p_seq = (seq_t)p;                   /* promote to seq_t */

    /* Compute 11*p^2 - 7*p = p*(11*p - 7) avoiding overflow in uint64_t
       for the intended test ranges. */
    seq_t ans = p_seq * ((p_seq * (seq_t)11) - (seq_t)7);
    return ans;
}
