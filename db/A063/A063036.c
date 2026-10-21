/*
 * A063036  Difference between average of smallest prime greater than n^3 and
 *          largest prime less than (n+1)^3 and n-th pronic [=n(n+1)].
 * https://oeis.org/A063036
 *
 * Offset:    2    (a(2) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = ((next_prime(n^3+1) + prev_prime((n+1)^3-1)) / 2) -
 *            n*(n+1) where next_prime(x) returns the smallest prime > x-1 and
 *            prev_prime(x) returns the largest prime ≤ x
 * Range:     u8 n <= 5, u16 n <= 39, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   next_prime, prev_prime
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include "../utils/next_prime.h"
#include "../utils/prev_prime.h"

seq_t a(idx_t n)
{
    if (n < 2) return 0; /* offset is 2, behaviour below that does not matter */

    /* n may be up to 64‑bit, but we only need 64‑bit arithmetic for the cubes */
    uint64_t n64 = (uint64_t)n;
    uint64_t n_cube = n64 * n64 * n64;          /* n^3   */
    uint64_t np1 = n64 + 1;
    uint64_t np1_cube = np1 * np1 * np1;        /* (n+1)^3 */

    /* smallest prime strictly greater than n^3 */
    uint64_t p1 = next_prime(n_cube + 1);

    /* largest prime strictly less than (n+1)^3 */
    uint64_t p2 = prev_prime(np1_cube - 1);

    /* pronic number n(n+1) in seq_t to avoid overflow */
    seq_t pronic = (seq_t)n * ((seq_t)n + 1);

    /* average of the two primes, integral by parity argument */
    seq_t avg = ((seq_t)p1 + (seq_t)p2) / 2;

    return avg - pronic;
}
