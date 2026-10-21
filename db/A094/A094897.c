/*
 * A094897  If 4*n+1 is not prime and 4*n+3 is prime then a(n)=4*n+3, else
 *          a(n)=0.
 * https://oeis.org/A094897
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 4*n+3 if 4*n+1 is composite and 4*n+3 is prime; otherwise
 *            a(n) = 0
 * Range:     u8 n <= 64, u16 n <= 83, u32 n <= 83, u64 n <= 83, u128 n <= 83,
 *            big any n
 * Helpers:   is_prime
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/is_prime.h"

seq_t a(idx_t n)
{
    /* Compute 4*n+1 and 4*n+3 using 64‑bit arithmetic.  The index n is
       guaranteed to be small enough so that these values fit in a uint64_t. */
    uint64_t v1 = 4ULL * n + 1ULL;   /* 4*n + 1  */
    uint64_t v2 = v1 + 2ULL;        /* 4*n + 3  */

    if (!is_prime(v1) && is_prime(v2))
        return (seq_t)v2;
    return (seq_t)0;
}
