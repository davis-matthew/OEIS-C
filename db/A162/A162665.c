/*
 * A162665  a(n) = sigma_2(n) + 3 sigma(n) tau(n) + tau(n)^3.
 * https://oeis.org/A162665
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = sigma_2(n) + 3 * sigma(n) * tau(n) + tau(n)^3
 * Range:     u8 n <= 5, u16 n <= 167, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   divisor_count_u64, sigma_k, sigma_of
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Here sigma_2(n) = sum of squares of divisors of n,
   sigma(n)   = sum of divisors of n,
   tau(n)     = number of divisors of n. */

#include "../utils/sigma_k.h"          /* sigma_k(idx_t m, uint8_t k) */
#include "../utils/sigma_of.h"         /* sigma_of(idx_t n) returns sigma(n) */
#include "../utils/divisor_count_u64.h"/* divisor_count_u64(uint64_t n) returns tau(n) */
#include <stdint.h>

seq_t a(idx_t n)
{
    /* The OEIS offset is 1; a(0) is never requested, but guard anyway. */
    if (n == 0)
        return 0;

    /* Compute each component using the provided utilities. */
    seq_t sigma2 = sigma_k(n, 2);                 /* σ₂(n) */
    seq_t sigma  = sigma_of(n);                   /* σ(n)  */
    seq_t tau    = (seq_t)divisor_count_u64(n);   /* τ(n)  */

    /* Final value: σ₂(n) + 3σ(n)τ(n) + τ(n)³ */
    seq_t term = sigma2
                 + 3 * sigma * tau
                 + tau * tau * tau;

    return term;
}
