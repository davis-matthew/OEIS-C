/*
 * A141663  Regular triangle T(n,m) = abs(prime(n) - m^2), 0 <= m <= n-1.
 * https://oeis.org/A141663
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy, tabl
 * Formula:   a(n) = |prime(k) - m²|, where k = smallest integer with k(k+1)/2
 *            ≥ n, and m = n - k(k-1)/2 - 1. (Row and column indices are
 *            0‑based.)
 * Range:     u8 n <= 188, u16 n <= 5050, u32 n <= 5050, u64 n <= 5050,
 *            u128 n <= 5050, big any n
 * Helpers:   get_prime
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/get_prime.h"

/* Computes the n‑th term of OEIS sequence A141663. */
seq_t a(idx_t n)
{
    if (n == 0)
        return 0;                /* undefined for offset 0, defensive return */

    /* Find the row number k such that k(k+1)/2 ≥ n. */
    idx_t k = 1;
    while ((k * (k + 1)) / 2 < n)
        ++k;

    /* Compute the column index m (0‑based). */
    idx_t prev = (k - 1) * k / 2;
    idx_t m = n - prev - 1;      /* 0 ≤ m < k */

    /* Retrieve the k‑th prime. */
    seq_t prime = (seq_t)get_prime(k);

    /* Compute m² in the same integer type. */
    seq_t sq = (seq_t)m * (seq_t)m;

    /* Return the absolute difference without using <math.h>. */
    return (prime > sq) ? (prime - sq) : (sq - prime);
}
