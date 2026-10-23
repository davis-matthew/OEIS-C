/*
 * A194699  a(n) = floor((p - 1)/12) - floor((p^2 - 1)/(24*p)), where p =
 *          prime(n).
 * https://oeis.org/A194699
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = floor((p-1)/12) - floor((p^2-1)/(24p)), where p =
 *            prime(n)
 * Range:     u8 n <= 54, u16 n <= 77, u32 n <= 77, u64 n <= 77, u128 n <= 77,
 *            big any n
 * Helpers:   get_prime
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include "../utils/get_prime.h"

seq_t a(idx_t n)
{
    if (n == 0)              /* undefined by OEIS, return 0 for safety */
        return 0;

    /* Retrieve the n‑th prime (1‑based).  get_prime returns a 32‑bit value. */
    uint32_t p32 = get_prime(n);

    /* Compute in seq_t to avoid overflow and to support arbitrary‑precision
       builds. */
    seq_t p      = (seq_t)p32;
    seq_t term1  = (p - (seq_t)1) / (seq_t)12;
    seq_t term2  = ((p * p) - (seq_t)1) / ((seq_t)24 * p);

    return term1 - term2;
}
