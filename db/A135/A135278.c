/*
 * A135278  Triangle read by rows, giving the numbers T(n,m) = binomial(n+1,
 *          m+1); or, Pascal's triangle A007318 with its left-hand edge
 *          removed.
 * https://oeis.org/A135278
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, nonn, tabl
 * Formula:   a(n) = binomial(r+1, c+1) where r = largest integer with
 *            r(r+1)/2 <= n c = n - r(r+1)/2 This reproduces the triangle read
 *            by rows T(n,k) = binomial(n+1, k+1)
 * Range:     u8 n <= 57, u16 n <= 177, u32 n <= 610, u64 n <= 2307,
 *            u128 n <= 5150, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>

/* Compute binomial coefficient C(n,k) as seq_t.
   The algorithm uses 128‑bit arithmetic for the intermediate
   multiplication to avoid overflow. */
static inline seq_t binom_compute(idx_t n, idx_t k)
{
    if (k > n)
        return 0;
    if (k > n - k)
        k = n - k;          /* use symmetry */
    seq_t res = 1;
    for (idx_t i = 1; i <= k; ++i) {
        idx_t numerator = n - k + i;
        /* 128‑bit product avoids overflow of seq_t */
        unsigned __int128 temp = (unsigned __int128)res * numerator;
        temp /= i;
        res = (seq_t)temp;
    }
    return res;
}

/* Main sequence function */
seq_t a(idx_t n)
{
    /* find row r such that r(r+1)/2 <= n < (r+1)(r+2)/2 */
    idx_t r = 0;
    while (((unsigned __int128)(r + 1) * (r + 2)) / 2 <= n)
        ++r;

    /* column index within the row */
    unsigned __int128 prev128 = (unsigned __int128)r * (r + 1) / 2;
    idx_t prev = (idx_t)prev128;
    idx_t c = n - prev;

    /* return the binomial coefficient for this (row, column) */
    return binom_compute(r + 1, c + 1);
}
