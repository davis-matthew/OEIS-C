/*
 * A180866  Square array read by antidiagonals: T(n,k) is the Wiener index of
 *          the firecracker graph F(n,k) (n>=1, k>=2). F(n,k) is the graph
 *          obtained by the concatenation of n k-stars by linking one leaf
 *          from each. The Wiener index of a connected graph is the sum of
 *          distances between all unordered pairs of v...
 * https://oeis.org/A180866
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, tabl
 * Formula:   T(n,k) = (1/6) * n * (6 + 6k - 7k^2 + n^2*k^2 + 12*n*k^2 -
 *            18*n*k) Sequence a(n) is read from the table T(n,k) along
 *            antidiagonals, starting with T(1,2) = 1
 * Range:     u8 n <= 16, u16 n <= 53, u32 n <= 53, u64 n <= 53, u128 n <= 53,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>

/* integer square root for unsigned __int128: floor(sqrt(x)) */
static unsigned __int128 isqrt128(unsigned __int128 x)
{
    if (x <= 3) return x;           /* small values */
    unsigned __int128 bit = (unsigned __int128)1 << 126;   /* largest power of 4 <= x */
    while (bit > x)
        bit >>= 2;
    unsigned __int128 r = 0;
    while (bit != 0) {
        if (x >= r + bit) {
            x -= r + bit;
            r = (r >> 1) + bit;
        } else {
            r >>= 1;
        }
        bit >>= 2;
    }
    return r;
}

/* Return the n-th term of the sequence.  idx_t is a 64‑bit unsigned type. */
seq_t a(idx_t n)
{
    /* 1.  Find the antidiagonal sum s = n + k of the term.  */
    /* The cumulative number of terms up to sum s is (s-2)(s-1)/2. */
    /* Solve for the smallest s such that (s-2)(s-1)/2 >= n. */

    /* Compute t: the largest integer satisfying t(t+1)/2 <= n. */
    unsigned __int128 x = (unsigned __int128)8 * n + 1;    /* 8n + 1 fits in 128 bits */
    unsigned __int128 r = isqrt128(x);                    /* floor sqrt(8n+1) */
    unsigned __int128 t0 = (r - 1) >> 1;                  /* floor((-1+sqrt)/2) */

    /* Check if we need to increase t to reach n */
    unsigned __int128 tri = t0 * (t0 + 1) / 2;
    unsigned __int128 t = t0;
    if (tri < n)
        ++t;

    unsigned __int128 s = t + 2;    /* the antidiagonal sum */

    /* 2.  Number of terms before this diagonal */
    unsigned __int128 prev = (s - 3) * (s - 2) / 2;

    /* 3.  Offset within the diagonal (0‑based), smallest n first */
    unsigned __int128 offset = (unsigned __int128)n - prev - 1;

    /* 4.  Coordinate on this diagonal */
    unsigned __int128 n_val = (s - 2) - offset;   /* n >= 1 */
    unsigned __int128 k_val = offset + 2;        /* k >= 2 */

    /* 5.  Compute T(n_val, k_val) using 128‑bit arithmetic */
    unsigned __int128 inner =
        (unsigned __int128)6
        + (unsigned __int128)6 * k_val
        - (unsigned __int128)7 * k_val * k_val
        + n_val * n_val * k_val * k_val
        + (unsigned __int128)12 * n_val * k_val * k_val
        - (unsigned __int128)18 * n_val * k_val;

    unsigned __int128 value = n_val * inner / 6;     /* exact division */

    return (seq_t)value;
}
