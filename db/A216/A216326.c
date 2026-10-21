/*
 * A216326  Number of divisors of the degree of the minimal polynomial of
 *          2*cos(Pi/prime(n)), with prime = A000040, n >= 1.
 * https://oeis.org/A216326
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = τ(δ(prime(n))) where τ = number of divisors, δ(p) = 1 if
 *            p = 2, otherwise (p‑1)/2
 * Range:     u8 n <= 91, u16 n <= 91, u32 n <= 91, u64 n <= 91, u128 n <= 91,
 *            big any n
 * Helpers:   divisor_count_u64, get_prime
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Helper functions provided by the build */
#include <stdint.h>
#include "../utils/divisor_count_u64.h"   /* divisor_count_u64(uint64_t) */
#include "../utils/get_prime.h"           /* get_prime(idx_t) */

seq_t a(idx_t n)
{
    /* Prime with 1‑based indexing: prime(1)=2, prime(2)=3, …  */
    uint32_t p = get_prime(n);

    uint64_t delta = 1;
    if (p > 2)
        delta = (uint64_t)(p - 1u) / 2u;   /* (p-1)/2 for odd primes */

    /* τ(δ) – number of divisors of δ */
    size_t tau = divisor_count_u64((uint64_t)delta);
    return (seq_t)tau;
}
