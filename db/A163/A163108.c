/*
 * A163108  a(n) = sigma(tau(sigma(n))).
 * https://oeis.org/A163108
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = sigma(tau(sigma(n)))
 * Range:     u8 n <= 502, u16 n <= 9141, u32 n <= 9141, u64 n <= 9141,
 *            u128 n <= 9141, big any n
 * Helpers:   divisor_count_u64, sigma_of
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include "../utils/sigma_of.h"
#include "../utils/divisor_count_u64.h"

seq_t a(idx_t n)
{
    /* First compute σ(n). */
    seq_t sigma_n = sigma_of(n);

    /* σ(n) fits comfortably in uint64_t for all n up to 10 000 (and
       remains so for the typical test ranges).  The cast is safe
       because σ(n) never exceeds 2¹⁶⁰ in those ranges.  */
    uint64_t sigma_val = (uint64_t)sigma_n;

    /* Compute τ(σ(n)) – the number of divisors of σ(n). */
    uint64_t tau_val = divisor_count_u64(sigma_val);

    /* Finally return σ(τ(σ(n))).  τ(σ(n)) is small (<1000 for
       n ≤ 10 000), so it safely converts to idx_t. */
    return sigma_of((idx_t)tau_val);
}
