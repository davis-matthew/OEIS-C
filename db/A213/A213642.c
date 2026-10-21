/*
 * A213642  a(n) = prime(odd part of n).
 * https://oeis.org/A213642
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = prime(odd part of n) (OEIS A213642)
 * Range:     u8 n <= 54, u16 n <= 6542, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   get_prime
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* This implementation utilizes the provided helper
   uint32_t get_prime(idx_t k) which returns the k‑th prime
   (prime(1)=2, prime(2)=3, …).  The odd part of n is obtained
   by repeatedly dividing by 2 until an odd number remains. */

#include "../utils/get_prime.h"

seq_t a(idx_t n)
{
    /* OEIS offset is 1; for n==0 we return 0 (undefined in the
       original sequence but harmless for internal use). */
    if (n == 0)
        return 0;

    /* Remove factors of 2 to obtain the odd part of n. */
    idx_t m = n;
    while ((m & 1) == 0)
        m >>= 1;

    /* m >= 1; get the m‑th prime. The helper returns a 32‑bit
       value which fits into seq_t for all n considered. */
    return (seq_t)get_prime(m);
}
