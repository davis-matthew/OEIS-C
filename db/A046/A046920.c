/*
 * A046920  Number of ways to express n as p+2a^2; p = 1 or prime, a >= 0.
 * https://oeis.org/A046920
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = number of ways to write n as p + 2a² with p = 1 or a
 *            prime, a ≥ 0
 * Range:     u8 n <= 10000, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   is_prime
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/is_prime.h"

seq_t a(idx_t n)
{
    seq_t count = 0;
    for (idx_t a = 0;; ++a) {
        /* Compute 2*a² in 128‑bit to avoid overflow. */
        unsigned __int128 two_a_sq = (unsigned __int128)2 * a * a;
        if (two_a_sq >= n) break;          /* further a will only increase the term */
        idx_t rest = n - (idx_t)two_a_sq;   /* remains a 64‑bit value */
        if (rest == 1 || is_prime(rest)) ++count;
    }
    return count;
}
