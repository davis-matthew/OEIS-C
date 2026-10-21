/*
 * A064919  a(n) = Min { k | A064916(k) = n }.
 * https://oeis.org/A064919
 *
 * Offset:    2    (a(2) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = n if n is prime; otherwise 2*n-2 (offset 2)
 * Range:     u8 n <= 128, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>

/* Very small deterministic prime test for 64‑bit indices. */
static bool is_prime(idx_t n)
{
    if (n < 2u) return false;
    if (n % 2u == 0u) return n == 2u;
    for (idx_t i = 3u; i <= n / i; i += 2u)
        if (n % i == 0u) return false;
    return true;
}

seq_t a(idx_t n)
{
    if (n < 2u) return 0;                 /* below offset – value irrelevant */
    if (is_prime(n))
        return (seq_t)n;
    return ((seq_t)n * (seq_t)2u) - (seq_t)2u;
}
