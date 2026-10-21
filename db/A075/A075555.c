/*
 * A075555  Smallest prime p such that p+n is a square, or 0 if no such p
 *          exists.
 * https://oeis.org/A075555
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   smallest prime p such that p + n is a perfect square; 0 if none
 *            exists
 * Range:     u8 n <= 62, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   floor_sqrt, is_prime_big
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>
#include "../utils/floor_sqrt.h"
#include "../utils/is_prime_big.h"

seq_t a(idx_t n)
{
    if (n == 0) return 0;            /* undefined for n < 1, safety guard */

    seq_t n_seq = (seq_t)n;

    /* Detect known zeros: n = m^2 with (2*m+1) composite (odd) */
    seq_t m = floor_sqrt(n_seq);
    if (m * m == n_seq)
    {
        seq_t k = (seq_t)2 * m + 1;
        if (!is_prime_big(k))
            return (seq_t)0;
    }

    /* Start searching from the smallest integer x with x^2 > n+1 (so that p >= 2) */
    seq_t x = floor_sqrt(n_seq + (seq_t)1) + (seq_t)1;

    /* Upper bound on search to avoid infinite loops (conservative for n ≤ 10000) */
    const seq_t MAX_EXTRA = (seq_t)1000000;
    seq_t limit = n_seq + MAX_EXTRA;

    for (; x <= limit; ++x)
    {
        seq_t p = x * x - n_seq;
        if (p < (seq_t)2)
            continue;
        if (is_prime_big(p))
            return p;
    }

    /* If no prime found within the bound, return 0 (should not happen for known n). */
    return (seq_t)0;
}
