/*
 * A212211  Triangle read by rows: T(n,k) = pi(n) + pi(k) - pi(n+k), n >= 2, 2
 *          <= k <= n, where pi() = A000720().
 * https://oeis.org/A212211
 *
 * Offset:    2    (a(2) is the first term)
 * Keywords:  nonn, tabl, nice
 * Formula:   a(n) = pi(m) + pi(k) - pi(m+k), * where (m,k) are derived from
 *            the index n by reading the * triangle T(n,k)=pi(n)+pi(k)-pi(n+k)
 *            row by row as * described in OEIS A212211
 * Range:     u8 n <= 9877, u16 n <= 9877, u32 n <= 9877, u64 n <= 9877,
 *            u128 n <= 9877, big any n
 * Helpers:   compute_pi
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include "../utils/compute_pi.h"

/* Return the nth term of OEIS sequence A212211.
 * The sequence is defined for n >= 2; values for lower n
 * are irrelevant and returned as 0. */
seq_t a(idx_t n)
{
    /* Offset: a(2) is the first listed term. */
    if (n < 2)
        return (seq_t)0;

    /* Find the row m such that m(m-1)/2 >= n-1.
     * Since the number of terms up to row m is m(m-1)/2,
     * this gives the correct row for index n. */
    idx_t x = n - 1;            /* n-1 is the local offset */
    idx_t m = 2;
    while ((m * (m - 1)) / 2 < x)
        ++m;

    /* Position of the term within row m (1‑based). */
    idx_t prev_sum = ((m - 1) * (m - 2)) / 2;
    idx_t position = x - prev_sum;   /* 1 .. (m-1) */

    /* Corresponding k value (runs from 2 to m). */
    idx_t k = position + 1;

    /* Compute pi(n) using the provided utility. */
    seq_t pi_m    = compute_pi(m);
    seq_t pi_k    = compute_pi(k);
    seq_t pi_sum  = compute_pi(m + k);

    /* Result of the triangle entry. */
    seq_t result = pi_m + pi_k - pi_sum;
    return result;
}
