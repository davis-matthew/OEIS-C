/*
 * A120456  Antidiagonal triangular version of the modulo 15 prime
 *          multiplication table past n=3.
 * https://oeis.org/A120456
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, tabf, fini
 * Formula:   a(n) = (b[i] * b[j]) mod 15, * where b = {1, 2, 4, 7, 8, 11, 13,
 *            14} (periodic of period 8). * The pair (i,j) is obtained by
 *            taking the n‑th element of the * anti–diagonal enumeration of
 *            the infinite table * T[n,m] = (b[n] * b[m]) mod 15.
 *            Specifically, * let k be the unique integer such that * k*...
 * Range:     u8 n <= 35, u16 n <= 35, u32 n <= 35, u64 n <= 35, u128 n <= 35,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

/* Compute floor(sqrt(x)) for unsigned 64‑bit integers. */
static inline idx_t floor_sqrt(idx_t x)
{
    idx_t r = 0;
    /* Find the greatest power of 4 <= x */
    idx_t bit = ((idx_t)1ULL) << 62; /* 4^(31) = 2^62 <= 2^64-1 */
    while (bit > x)
        bit >>= 2;
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

seq_t a(idx_t n)
{
    /* Periodic list of prime digit endings mod 15 */
    static const seq_t b[8] = {1, 2, 4, 7, 8, 11, 13, 14};

    /* Find the diagonal k such that n is in it. */
    /* k is the unique integer with k*(k+1)/2 <= n < (k+1)*(k+2)/2. */
    /* Compute k using integer sqrt: k = (floor(sqrt(1+8*n)) - 1) / 2. */
    unsigned __int128 tmp = (unsigned __int128)8 * n + 1;
    idx_t d = floor_sqrt((idx_t)tmp);
    idx_t k = (d - 1) / 2;

    /* Start index of this diagonal */
    idx_t start = k * (k + 1) / 2;
    idx_t offset = n - start;

    idx_t i = offset;           /* row index within diagonal */
    idx_t j = k - offset;       /* column index within diagonal */

    /* Reduce i and j modulo 8 for the periodic sequence b. */
    idx_t i_mod = i & 7;        /* equivalent to i % 8 since 8 is power of two */
    idx_t j_mod = j & 7;

    /* Compute T[i_mod][j_mod] = (b[i_mod] * b[j_mod]) mod 15. */
    seq_t prod = (seq_t)b[i_mod] * (seq_t)b[j_mod];
    seq_t val = prod % 15;

    return val;
}
