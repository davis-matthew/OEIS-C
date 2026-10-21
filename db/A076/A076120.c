/*
 * A076120  a(n) = smallest multiple of n other than n which is relatively
 *          prime to both a(n-1) and a(n+1).
 * https://oeis.org/A076120
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) computed as follows: - a(1) = 3 (base case) - If n is even:
 *            a(n) = 2*n - If n is odd (n ≥ 3): a(n) = n * k where k is the
 *            smallest odd integer k ≥ 3 such that gcd(k, n-1) = gcd(k, n+1) =
 *            1. This satisfies the definition that a(n) is the smallest
 *            multiple of n (other than n itself) which is r...
 * Range:     u8 n <= 28, u16 n <= 61, u32 n <= 61, u64 n <= 61, u128 n <= 61,
 *            big any n
 * Helpers:   gcd
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include "../utils/gcd.h"

seq_t a(idx_t n)
{
    if (n == 1)
        return (seq_t)3;

    /* Even indices: always a(n) = 2*n. */
    if ((n & 1) == 0)
        return (seq_t)n * 2;

    /* Odd indices n ≥ 3.  Find the smallest odd k ≥ 3 such that
       gcd(k, n-1) = gcd(k, n+1) = 1.  Then a(n) = n*k. */
    idx_t k = 3;
    for (;; k += 2) {
        seq_t ks = (seq_t)k;
        if (gcd(ks, (seq_t)(n - 1)) == 1 &&
            gcd(ks, (seq_t)(n + 1)) == 1)
            return (seq_t)n * ks;
    }
}
