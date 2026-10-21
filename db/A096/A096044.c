/*
 * A096044  Triangle read by rows: T(n,k) = (n+1,k)-th element of (M^10-M)/9,
 *          where M is the infinite lower Pascal's triangle matrix, 1<=k<=n.
 * https://oeis.org/A096044
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, tabl
 * Formula:   a(n) = binomial(r, k‑1) * (10^(r‑k+1) – 1)/9, where r and k
 *            satisfy n = (r‑1)·r/2 + k (1‑based indices)
 * Range:     u8 n <= 6, u16 n <= 15, u32 n <= 55, u64 n <= 191, u128 n <= 742,
 *            big any n
 * Helpers:   binomial
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include "../utils/binomial.h"

seq_t a(idx_t n)
{
    /* Find the row r containing the n-th entry. */
    idx_t r = 1;
    while (1) {
        idx_t tot = r * (r + 1) / 2;        /* total entries up to row r */
        if (n <= tot)
            break;
        r++;
    }

    /* Entries before row r */
    idx_t prev_tot = (r - 1) * r / 2;

    /* Column within the current row (1‑based). */
    idx_t k = n - prev_tot;                 /* 1 ≤ k ≤ r */

    /* Binomial coefficient C(r, k‑1). */
    seq_t bin = binomial(r, k - 1);

    /* Length of the repunit part: exp = r‑k+1. */
    idx_t exp = r - k + 1;
    seq_t rep = 0;
    for (idx_t i = 0; i < exp; ++i)
        rep = rep * 10 + 1;

    return bin * rep;
}
