/*
 * A159919  A square array of numbers, read by antidiagonals, called
 *          Sundaram's sieve.
 * https://oeis.org/A159919
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  easy, nonn, tabl
 * Formula:   not stated
 * Range:     u8 n <= 218, u16 n <= 8375, u32 n <= 8375, u64 n <= 8375,
 *            u128 n <= 8375, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computes A159919: a(n) is the antidiagonal traversal of the array
 * T(j,k) = 2*j*k + j + k for j,k ≥ 1.  For a given n (n ≥ 1) the
 * pair (j,k) is found by determining the diagonal s = j + k that
 * contains the element with that index, and then returning T(j,k).
 * The diagonal order is j = 1 .. s-1, so within each diagonal the
 * pairs are listed with increasing j. */

#include <stdint.h>

/* external types supplied by the build (do NOT redeclare them) */
seq_t a(idx_t n)
{
    /* 1‑based sequence; return 0 safely for n == 0. */
    if (n == 0) return 0;

    /* Find the smallest diagonal index s such that C(s) = (s-1)*s/2 ≥ n. */
    idx_t s = 2;
    for (;;) {
        unsigned __int128 cum = (unsigned __int128)(s - 1) * s / 2;
        if (cum >= n) break;
        ++s;
    }

    /* Elements before diagonal s: C(s-1) = (s-2)*(s-1)/2. */
    unsigned __int128 prev_cum = (unsigned __int128)(s - 2) * (s - 1) / 2;
    idx_t prev = (idx_t)prev_cum;

    /* 0‑based offset within diagonal s. */
    idx_t offset = n - prev - 1;

    /* Coordinates on this diagonal. */
    idx_t j = offset + 1;      /* 1 ≤ j ≤ s-1 */
    idx_t k = s - j;           /* k ≥ 1 */

    /* Compute T(j,k) = 2*j*k + j + k using 128‑bit arithmetic to avoid
     * overflow before casting to seq_t. */
    unsigned __int128 val128 = (unsigned __int128)2 * j * k + j + k;
    return (seq_t)val128;
}
