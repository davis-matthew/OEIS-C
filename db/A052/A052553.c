/*
 * A052553  Square array of binomial coefficients T(n,k) = binomial(n,k), n >=
 *          0, k >= 0, read by upward antidiagonals.
 * https://oeis.org/A052553
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, tabl, easy, nice
 * Formula:   a(n) = binomial(r, c) with s = the unique integer satisfying
 *            s*(s+1)/2 ≤ n < (s+1)*(s+2)/2 t = n – s*(s+1)/2 r = s - t, c = t
 *            (binomial is defined as 0 when c > r)
 * Range:     u8 n <= 123, u16 n <= 384, u32 n <= 1338, u64 n <= 4875,
 *            u128 n <= 5459, big any n
 * Helpers:   gcd64
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* <stdint.h> is included for the 64‑bit types used in the helper functions. */
#include <stdint.h>
#include "../utils/gcd64.h"

/* Euclidean algorithm for seq_t (works for built‑in integer types and
   arbitrary‑precision integer classes). */
static inline seq_t gcd_seq(seq_t a, seq_t b)
{
    while (b)
    {
        seq_t t = b;
        b = a % b;
        a = t;
    }
    return a;
}

/* Euclidean algorithm for 64‑bit unsigned integers. */

/* Exact (non‑overflowing as far as seq_t permits) binomial coefficient
   computed by successive cancellation. */
static inline seq_t binom_exact(idx_t n, idx_t k)
{
    if (k > n)
        return (seq_t)0;
    if (k > n - k)
        k = n - k;
    seq_t res = (seq_t)1;
    for (idx_t i = 1; i <= k; ++i)
    {
        seq_t num = (seq_t)(n - k + i);       /* numerator: <= n      */
        idx_t  den = i;                       /* denominator: <= k    */

        /* Cancel common factors between numerator and denominator. */
        uint64_t g1 = gcd64((uint64_t)num, den);
        num /= g1;
        den /= g1;

        /* Cancel common factors between current result and denominator. */
        seq_t g2 = gcd_seq(res, (seq_t)den);
        res /= g2;
        den /= (idx_t)g2;                     /* den is now 1 */

        /* Multiply by the reduced numerator. */
        res *= num;
    }
    return res;
}

/* Main sequence generator. */
seq_t a(idx_t n)
{
    /* Find the diagonal index 's' such that s*(s+1)/2 ≤ n < (s+1)*(s+2)/2. */
    idx_t s = 0;
    while (((unsigned __int128)(s + 1) * (s + 2)) / 2 <= n)
        ++s;

    /* Compute offset within this diagonal. */
    idx_t prev = ((unsigned __int128)s * (s + 1)) / 2;
    idx_t t    = n - prev;

    /* Convert to row/column indices in the Pascal triangle. */
    idx_t row = s - t;   /* n' */
    idx_t col = t;       /* k  */

    return binom_exact(row, col);
}
