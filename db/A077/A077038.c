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
 * Validated: standard, pass, 90 terms through n = 9225 (2026-11-01)
 * Helpers:   is_prime, next_prime
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include <stdint.h>
#include "../utils/is_prime.h"
#include "../utils/next_prime.h"

/* Return the largest prime < n   (n > 2)   ; pp(2) = 1, but we never call it for n==2 */
static inline idx_t prev_prime(idx_t n)
{
    idx_t p = n - 1;
    for (;; --p)
    {
        if (is_prime(p)) return p;
    }
}

seq_t a(idx_t n) {
    /* n ≥ 2 (offset) */
    idx_t cube = n * n * n;            /* n³ fits in 64‑bit for n ≤ 10⁴ */
    idx_t left  = prev_prime(cube);    /* largest prime ≤ n³  (n³ is composite for n≥2) */
    idx_t right = next_prime(cube);    /* smallest prime > n³     */
    return (seq_t)right - (seq_t)left;
}
