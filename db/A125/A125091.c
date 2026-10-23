/*
 * A125091  Triangle read by rows: T(n,k) = (1/6)*k*(k+1)*(k+2)*binomial(n,k)
 *          (1 <= k <= n).
 * https://oeis.org/A125091
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, tabl
 * Formula:   a(n) = T(r,k) = (1/6) * k * (k+1) * (k+2) * binomial(r,k)
 * Range:     u8 n <= 18, u16 n <= 56, u32 n <= 56, u64 n <= 56, u128 n <= 56,
 *            big any n
 * Helpers:   binomial
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* The row r and column k are recovered from the index n by */
/* solving (r-1)*r/2 < n ≤ r*(r+1)/2. */

#include "../utils/binomial.h"

seq_t a(idx_t n)
{
    /* offset is 1 – undefined for n < 1 */
    if (n < 1)
        return 0;

    /* Find the row r such that cumulative terms up to r >= n. */
    idx_t r = 1;
    idx_t cum = 1;                     /* cumulative terms up to row r */
    while (n > cum) {
        ++r;
        cum += r;
    }

    /* Previous cumulative total before row r */
    idx_t prev_cum = cum - r;
    /* Column index k within row r (1‑based) */
    idx_t k = n - prev_cum;

    /* Compute binomial coefficient binomial(r,k) */
    seq_t binom = binomial(r, k);

    /* Compute k*(k+1)*(k+2)/6 using a 128‑bit intermediate to avoid overflow */
    unsigned __int128 tmp = (unsigned __int128)k * (k + 1) * (k + 2);
    tmp /= 6;
    seq_t mult = (seq_t)tmp;

    /* Final term = binom * mult */
    return binom * mult;
}
