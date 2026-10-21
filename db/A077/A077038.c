/*
 * A077038  Least difference of primes p, q such that p < n^3 < q.
 * https://oeis.org/A077038
 *
 * Offset:    2    (a(2) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = next_prime(n³) - prev_prime(n³) where prev_prime returns
 *            the largest prime ≤ x and next_prime returns the smallest prime
 *            > x. This gives the least difference of primes p< n³ < q
 * Range:     u8 n <= 10000, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   next_prime, prev_prime
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/prev_prime.h"
#include "../utils/next_prime.h"

seq_t a(idx_t n) {
    /* n ≥ 2 (offset) */
    idx_t cube = n * n * n;            /* n³ fits in 64‑bit for n ≤ 10⁴ */
    idx_t left  = prev_prime(cube);    /* largest prime ≤ n³  (n³ is composite for n≥2) */
    idx_t right = next_prime(cube);    /* smallest prime > n³     */
    return (seq_t)right - (seq_t)left;
}
