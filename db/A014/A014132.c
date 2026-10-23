/*
 * A014132  Complement of triangular numbers (A000217); also array T(n,k) =
 *          ((n+k)^2 + n-k)/2, n, k > 0, read by antidiagonals.
 * https://oeis.org/A014132
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy, nice, tabl
 * Formula:   a(n) = n + round(sqrt(2*n))
 * Range:     u8 n <= 31, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   floor_sqrt
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* This implementation uses integer arithmetic only and relies on the
   supplied floor_sqrt(seq_t) routine.  It works for any seq_t that
   supports the usual arithmetic operators and is large enough to hold
   the result.  For 64‑bit seq_t, 128‑bit intermediate arithmetic is
   used to avoid overflow when computing t*t. */

#include "../utils/floor_sqrt.h"

/* a(n): the n‑th positive integer that is NOT a triangular number */
seq_t a(idx_t n)
{
    /* The sequence is defined for n ≥ 1.  The behavior for n = 0 is
       irrelevant in the OEIS context; return 0 to avoid UB. */
    if (n == 0)
        return 0;

    seq_t N = (seq_t)n;          /* convert idx_t → seq_t once */
    seq_t k = N + N;             /* k = 2*n, fits in seq_t */

    /* t = floor(sqrt(k)) */
    seq_t t = floor_sqrt(k);

    /* Compute r = k - t*t without overflow:
       use 128‑bit arithmetic for the product, then back to seq_t. */
    unsigned __int128 tk = (unsigned __int128)t * (unsigned __int128)t;
    unsigned __int128 kk = (unsigned __int128)k;
    unsigned __int128 r  = kk - tk;          /* 0 ≤ r < 2*t */

    /* Round up iff r > t.  This is equivalent to the standard rounding
       of sqrt(k). */
    seq_t inc = (seq_t)((r > (unsigned __int128)t) ? 1U : 0U);

    return N + t + inc;
}
