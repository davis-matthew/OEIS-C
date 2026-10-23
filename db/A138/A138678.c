/*
 * A138678  Prime(n)^9 mod prime(n-1).
 * https://oeis.org/A138678
 *
 * Offset:    2    (a(2) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = prime(n)^9 mod prime(n-1)
 * Range:     u8 n <= 60, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   get_prime, mod_pow
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/get_prime.h"
#include "../utils/mod_pow.h"

seq_t a(idx_t n)
{
    if (n < 2) return 0;  /* undefined for n < 2, but safe */
    seq_t p  = (seq_t)get_prime(n);   /* prime(n) */
    seq_t pm = (seq_t)get_prime(n-1); /* prime(n‑1) */
    return mod_pow(p, 9ULL, pm);
}
