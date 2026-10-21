/*
 * A212210  Triangle read by rows: T(n,k) = pi(n) + pi(k) - pi(n+k), n >= 1, 1
 *          <= k <= n, where pi() = A000720().
 * https://oeis.org/A212210
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  sign, tabl, nice
 * Formula:   a(n) = pi(r) + pi(k) – pi(r + k) where r and k satisfy n =
 *            r*(r‑1)/2 + k, 1 ≤ k ≤ r. The pair (r,k) is found by triangular
 *            indexing of the rows of the triangle
 * Range:     i8 n <= 9985, i16 n <= 9985, i32 n <= 9985, i64 n <= 9985,
 *            i128 n <= 9985, big any n
 * Helpers:   prime_pi
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/prime_pi.h"

seq_t a(idx_t n)
{
    /* Find the smallest r with r*(r+1)/2 ≥ n. */
    idx_t low  = 1;
    idx_t high = n;

    while (low < high) {
        idx_t mid = low + (high - low) / 2;
        unsigned __int128 tri = (unsigned __int128)mid * (mid + 1) / 2;
        if (tri >= n)
            high = mid;
        else
            low = mid + 1;
    }
    idx_t r = low;

    /* Compute k = n – (r‑1)*r/2. */
    unsigned __int128 tri_before = (unsigned __int128)(r - 1) * r / 2;
    idx_t k = n - (idx_t)tri_before;          /* 1 ≤ k ≤ r */

    /* Evaluate T(r,k). */
    seq_t val = prime_pi(r) + prime_pi(k) - prime_pi(r + k);
    return val;
}
