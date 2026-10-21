/*
 * A078724  a(n) = prime(n*(n+1)/2+3).
 * https://oeis.org/A078724
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = prime(n*(n+1)/2 + 3)
 * Range:     u8 n <= 9, u16 n <= 113, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   get_prime
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*          (prime() is 1‑based) */
#include "../utils/get_prime.h"

seq_t a(idx_t n)
{
    seq_t idx = ((seq_t)n * ((seq_t)n + 1)) / 2 + 3;
    return (seq_t)get_prime((idx_t)idx);
}
