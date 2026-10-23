/*
 * A138679  Prime(n)^10 mod prime(n-1).
 * https://oeis.org/A138679
 *
 * Offset:    2    (a(2) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = prime(n)^10 mod prime(n-1) (offset 2)
 * Range:     u8 n <= 68, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   get_prime, mod_pow
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include "../utils/get_prime.h"   /* get_prime(idx_t k) → uint32_t */
#include "../utils/mod_pow.h"    /* mod_pow(seq_t base, uint64_t exp, seq_t mod) */

seq_t a(idx_t n)
{
    /* Undefined for n < 2; return 0. */
    if (n < 2)
        return (seq_t)0;

    /* Retrieve the required primes.  get_prime(k) gives the k‑th prime
       (1‑based).  For n ≥ 2, n-1 ≥ 1, so the call is safe. */
    seq_t pn   = (seq_t)get_prime(n);   /* prime(n)  */
    seq_t pn_1 = (seq_t)get_prime(n - 1); /* prime(n-1) */

    /* Compute pn^10 mod pn_1.  mod_pow handles the modular exponentiation
       safely for the chosen seq_t size. */
    return mod_pow(pn, 10, pn_1);
}
