/*
 * A084856  a(n) = prime(n+2)^2 - prime(n)^2.
 * https://oeis.org/A084856
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = prime(n+2)^2 - prime(n)^2
 * Range:     u8 n <= 7, u16 n <= 45, u32 n <= 45, u64 n <= 45, u128 n <= 45,
 *            big any n
 * Helpers:   get_prime
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computes a(n) for the OEIS sequence A084856. 
   Uses the 1‑based get_prime() helper from utils/ to obtain the primes.
   The function works for any n ≥ 1 and is safe for seq_t of any size. */
#include "../utils/get_prime.h"

seq_t a(idx_t n)
{
    /* get_prime is 1‑based: get_prime(1)=2, get_prime(2)=3, etc. */
    seq_t p_hi = (seq_t)get_prime(n + 2); /* prime(n+2) */
    seq_t p_lo = (seq_t)get_prime(n);     /* prime(n)   */

    /* Square the primes.  Multiplication is performed in seq_t to avoid
       overflow when seq_t is larger than 32 bits. */
    seq_t hi_sq = p_hi * p_hi;
    seq_t lo_sq = p_lo * p_lo;

    return hi_sq - lo_sq;
}
