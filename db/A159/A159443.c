/*
 * A159443  Number of n X n arrays of squares of integers summing to 39.
 * https://oeis.org/A159443
 *
 * Offset:    2    (a(2) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = number of n×n arrays of squares of integers summing to
 *            39. Computation by enumerating all multisets of nonzero squares
 *            whose sum is 39, then counting the arrangements of these squares
 *            among the n² positions
 * Range:     u8 n <= 2, u16 n <= 3, u32 n <= 4, u64 n <= 7, u128 n <= 12,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>

/* Forward declaration of helper */
static void gen_rec(idx_t idx, int remSum, int *counts, idx_t L, seq_t *total);

/* External–linkage sequence function */
seq_t a(idx_t n)
{
    if (n < 2) return 0;        /* sequence undefined below offset */

    /* total number of positions in the array */
    idx_t L = n * n;

    seq_t total = 0;
    int counts[6] = {0};

    /* start recursive generation with the largest available square */
    gen_rec(0, 39, counts, L, &total);

    return total;
}

/* Recursively generate all solutions for the non‑zero squares.
   weights[0] … weights[5] correspond to 36, 25, 16, 9, 4, 1 respectively. */
static void gen_rec(idx_t idx, int remSum, int *counts, idx_t L, seq_t *total)
{
    static const int weights[6] = {36, 25, 16, 9, 4, 1};

    if (idx == 6) {
        /* all weights processed: remSum must be zero */
        if (remSum != 0) return;

        /* Compute multinomial coefficient for this particular solution */
        seq_t ways = 1;
        idx_t remaining = L;

        for (idx_t i = 0; i < 6; ++i) {
            int c = counts[i];
            if (c == 0) continue;

            /* Compute C(remaining, c) */
            seq_t comb = 1;
            for (int t = 1; t <= c; ++t) {
                comb = comb * (seq_t)(remaining - t + 1) / (idx_t)t;
            }

            ways *= comb;
            remaining -= c;
        }

        /* Add to running total */
        *total += ways;
        return;
    }

    int w = weights[idx];
    int maxc = remSum / w;
    for (int c = 0; c <= maxc; ++c) {
        counts[idx] = c;
        gen_rec(idx + 1, remSum - c * w, counts, L, total);
    }
}
