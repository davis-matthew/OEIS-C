/*
 * A141724  Triangle T(n, k) = Sum_{m=0..k} Sum_{j=0..m} Multinomial(n-k-m-j,
 *          j, m, k), read by rows.
 * https://oeis.org/A141724
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, tabl
 * Formula:   not stated
 * Range:     u8 n <= 22, u16 n <= 57, u32 n <= 175, u64 n <= 638,
 *            u128 n <= 1325, big any n
 * Helpers:   binomial
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* 
 * Formula: a(n) is the n-th term of the flattened triangle T, defined by
 *   T(n,k) = Σ_{m=0..k} Σ_{j=0..m} multinomial(n-k-m-j,j,m,k)
 * The sequence a(n) is obtained by reading T row‑by‑row, i.e. 
 *  a(0)=T(0,0), a(1)=T(1,0), a(2)=T(1,1), a(3)=T(2,0), ….
 */

#include <stdint.h>
#include <stdbool.h>
#include "../utils/binomial.h"

/* return the n-th flattened triangle term */
seq_t a(idx_t n)
{
    /* locate row r and column k of the flattened index n */
    idx_t r = 0;           /* current row number */
    idx_t cumulative = 0;  /* total elements processed so far */

    /* increment rows until we pass n */
    while (cumulative <= n) {
        cumulative += r + 1;  /* elements in current row r */
        r += 1;
    }
    r -= 1;  /* last row that starts before or at position n */

    /* elements up to the previous row */
    idx_t prev_cumulative = r * (r + 1) / 2;
    idx_t k = n - prev_cumulative;  /* column inside row r  (0‑based) */

    /* compute T(r,k) */
    seq_t result = 0;

    for (idx_t m = 0; m <= k; ++m) {
        /* a = r - k - m - j   must be non‑negative */
        if (r < k + m) continue;        /* then a would be negative for all j */
        idx_t max_j = r - k - m;        /* because j ≤ m and j ≤ max_j */
        if (max_j > m) max_j = m;       /* j ≤ m constraint */
        for (idx_t j = 0; j <= max_j; ++j) {
            idx_t a = r - k - m - j;     /* guaranteed non‑negative */

            /* multinomial = binomial(r, a) * binomial(r - a, j) *
                           binomial(r - a - j, m) */
            seq_t term = binomial(r, a);
            term = term * binomial(r - a, j);
            term = term * binomial(r - a - j, m);

            result += term;
        }
    }

    return result;
}
