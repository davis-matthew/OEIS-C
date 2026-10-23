/*
 * A118976  Triangle read by rows: T(n,k) =
 *          binomial(n-1,k-1)*binomial(n,k-1)/k +
 *          binomial(n-1,k)*binomial(n,k)/(k+1) (1 <= k <= n). In other words,
 *          to each entry of the Narayana triangle (A001263) add the entry on
 *          its right.
 * https://oeis.org/A118976
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, tabl
 * Formula:   a(n) = T(r, k) where T(r,k) =
 *            binomial(r-1,k-1)*binomial(r,k-1)/k +
 *            binomial(r-1,k)*binomial(r,k)/(k+1), and a is the triangle read
 *            by rows, 1‑based indexing
 * Range:     u8 n <= 23, u16 n <= 70, u32 n <= 218, u64 n <= 718,
 *            u128 n <= 2448, big any n
 * Helpers:   binom
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>
#include <stdlib.h>
#include "../utils/binom.h"

seq_t a(idx_t n) {
    /* Reject n < 1; behavior below the offset does not matter */
    if (n < 1) return 0;

    /* Find the row r such that r(r+1)/2 >= n */
    idx_t low = 1, high = n;
    while (low < high) {
        idx_t mid = low + (high - low) / 2;
        idx_t tri = mid * (mid + 1) / 2;
        if (tri >= n)
            high = mid;
        else
            low = mid + 1;
    }
    idx_t r = low;

    /* Position k in row r (1‑based) */
    idx_t prev_tri = (r - 1) * r / 2;
    idx_t k = n - prev_tri;  /* 1 ≤ k ≤ r */

    /* Compute the two terms of the definition safely */
    seq_t term1;
    {
        seq_t c1 = binom(r - 1, k - 1);
        seq_t c2 = binom(r, k - 1);
        /* For built‑in integer types, use 128‑bit intermediate to avoid overflow */
        if (sizeof(seq_t) <= sizeof(uint64_t)) {
            unsigned __int128 prod = (unsigned __int128)c1 * (unsigned __int128)c2;
            term1 = (seq_t)(prod / k);
        } else {
            term1 = (c1 * c2) / (seq_t)k;
        }
    }

    seq_t term2;
    {
        seq_t c1 = binom(r - 1, k);
        seq_t c2 = binom(r, k);
        if (sizeof(seq_t) <= sizeof(uint64_t)) {
            unsigned __int128 prod = (unsigned __int128)c1 * (unsigned __int128)c2;
            term2 = (seq_t)(prod / (k + 1));
        } else {
            term2 = (c1 * c2) / (seq_t)(k + 1);
        }
    }

    return term1 + term2;
}
