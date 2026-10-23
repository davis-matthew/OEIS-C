/*
 * A178180  Numbers k such that prime(2k) mod prime(k) = prime(2k+2) mod
 *          prime(k+1).
 * https://oeis.org/A178180
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 22, u16 n <= 54, u32 n <= 54, u64 n <= 54, u128 n <= 54,
 *            big any n
 * Helpers:   get_prime
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* A178180: a(n) = the n‑th integer k (n ≥ 1) such that
   prime(2k) mod prime(k) == prime(2k+2) mod prime(k+1).
   The sequence starts a(1)=1.  The code evaluates the
   criterion directly for each candidate k; no table is stored. */

#include <stdint.h>
#include "../utils/get_prime.h"            /* uint32_t get_prime(idx_t) */

/* Return the n‑th term of A178180 (1‑based index).  */
seq_t a(idx_t n)
{
    if (n == 0)
        return (seq_t)0;   /* undefined for n=0 – return 0 silently */

    idx_t k = 1;          /* candidate k */
    idx_t count = 0;      /* how many k satisfy the condition so far */

    for (;; ++k)
    {
        /* Obtain the required primes.  */
        uint32_t pk   = get_prime(k);
        uint32_t pk1  = get_prime(k + 1);
        uint32_t p2k  = get_prime(2 * k);
        uint32_t p2k2 = get_prime(2 * k + 2);

        /* Compare remainders.  */
        uint64_t r1 = (uint64_t)p2k  % pk;
        uint64_t r2 = (uint64_t)p2k2 % pk1;

        if (r1 == r2)
        {
            ++count;
            if (count == n)
                return (seq_t)k;   /* found the n‑th term */
        }
    }
}
