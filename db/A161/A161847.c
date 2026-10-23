/*
 * A161847  Denominator of the ratio (prime((n+1)^2) - prime(n^2))/prime(n).
 * https://oeis.org/A161847
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, frac
 * Formula:   a(n) = denominator((prime((n+1)^2) - prime(n^2))/prime(n))
 * Range:     u8 n <= 54, u16 n <= 59, u32 n <= 59, u64 n <= 59, u128 n <= 59,
 *            big any n
 * Helpers:   gcd, get_prime
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/gcd.h"
#include "../utils/get_prime.h"

seq_t a(idx_t n)
{
    if (n == 0) return 0;                     /* sequence is defined for n ≥ 1 */

    /* Prime at index n (the n‑th prime, 1‑based). */
    seq_t prime_n = (seq_t)get_prime(n);

    /* Indices for n² and (n+1)². */
    idx_t idx_n2    = n * n;
    idx_t idx_np1_2 = (n + 1) * (n + 1);

    /* Corresponding primes. */
    seq_t p_n2    = (seq_t)get_prime(idx_n2);
    seq_t p_np12  = (seq_t)get_prime(idx_np1_2);

    /* Numerator of the fraction: difference of the two primes. */
    seq_t diff = p_np12 > p_n2 ? p_np12 - p_n2 : p_n2 - p_np12;  /* always positive */

    /* Reduce the fraction: denominator = prime_n / gcd(diff, prime_n). */
    seq_t g = gcd(diff, prime_n);
    return prime_n / g;
}
