/*
 * A057022  a(n) = floor((sum of divisors of n) / (number of divisors of n)),
 *          or floor(sigma_1(n)/sigma_0(n)).
 * https://oeis.org/A057022
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = floor(σ₁(n)/τ(n)) where σ₁ is the sum of divisors * and τ
 *            is the number of divisors. The implementation uses the *
 *            provided utilities `sigma_of` and `divisor_count_u64`
 * Range:     u8 n <= 237, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   divisor_count_u64, sigma_of
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>                  /* for size_t */
#include "../utils/sigma_of.h"        /* seq_t sigma_of(idx_t n);        */
#include "../utils/divisor_count_u64.h" /* uint64_t divisor_count_u64(uint64_t n); */

seq_t a(idx_t n)
{
    /* By definition the sequence starts at n = 1.  For n = 0 return 0
     * safely (the caller must use only n ≥ 1). */
    if (n == 0)
        return 0;

    /* Compute the sum of divisors using the utility that works with
     * whatever `seq_t` type is chosen for the build. */
    seq_t sum = sigma_of(n);

    /* Compute the number of divisors.  The utility returns a 64‑bit
     * unsigned integer; cast it to `seq_t` so the division below uses
     * the appropriate integer type. */
    seq_t tau = (seq_t)divisor_count_u64((uint64_t)n);

    /* Integer division truncates towards zero, giving the floor. */
    return sum / tau;
}
