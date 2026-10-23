/*
 * A187873  Second smallest prime after n^2.
 * https://oeis.org/A187873
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = second smallest prime > n^2
 * Range:     u8 n <= 15, u16 n <= 254, u32 n <= 69, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   is_prime_big
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/is_prime_big.h"

seq_t a(idx_t n)
{
    /* compute n^2 as 64‑bit value (n ≤ 10000 in tests) */
    uint64_t sq = (uint64_t)n * (uint64_t)n;

    uint64_t cand = sq + 1;     /* start searching just above n^2 */
    int seen = 0;               /* count of primes found so far */

    for (;; ) {
        if (is_prime_big((seq_t)cand)) {
            seen++;
            if (seen == 2)        /* second prime after n^2 */
                return (seq_t)cand;
        }
        cand++;                   /* next integer to test */
    }
}
