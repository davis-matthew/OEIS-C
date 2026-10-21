/*
 * A053726  "Flag numbers": number of dots that can be arranged in successive
 *          rows of K, K-1, K, K-1, K, ..., K-1, K (assuming there is a total
 *          of L > 1 rows of size K > 1).
 * https://oeis.org/A053726
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = the n‑th integer m > 1 such that (2m‑1) is composite.
 *            Computation is done by a simple search which tests each
 *            candidate for primality of 2m‑1
 * Range:     u8 n <= 158, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Standard type headers. */
#include <stdint.h>
#include <stdbool.h>

/* ------------------------------------------------------------
   Small, deterministic primality test for 64‑bit unsigned integers.
   For numbers arising in this sequence (up to about 2 × 10⁴ for
   n ≤ 10 000) a simple trial division to √n is very fast.
   ------------------------------------------------------------ */
static bool is_prime_uint64(uint64_t n)
{
    if (n < 2ULL)      return false;
    if (n == 2ULL)     return true;
    if ((n & 1ULL) == 0ULL)  return false;   /* even numbers >2 */

    for (uint64_t d = 3ULL; d * d <= n; d += 2ULL) {
        if (n % d == 0ULL)  return false;
    }
    return true;
}

/* ------------------------------------------------------------
   Main sequence function: return the n‑th flag number.
   ------------------------------------------------------------ */
seq_t a(idx_t n)
{
    if (n < 1ULL)            /* undefined for indices below the offset */
        return (seq_t)0;

    idx_t count = 0ULL;      /* how many flags found so far */
    idx_t m = 2ULL;          /* candidate numbers start from 2 */

    while (true) {
        /* Check primality of 2m‑1; if composite, m is a flag number. */
        uint64_t test = 2ULL * m - 1ULL;
        if (!is_prime_uint64(test)) {
            ++count;
            if (count == n)  /* nth flag number reached */
                return (seq_t)m;
        }
        ++m;
    }
}
