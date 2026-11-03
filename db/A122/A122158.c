/*
 * A122158  Smallest positive number of "triangular" shuffles of n(n+1)/2
 *          cards needed to restore them to their original order.
 * https://oeis.org/A122158
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = order of the permutation induced by the triangular *
 *            shuffle on n(n+1)/2 cards
 * Range:     u8 n <= 9, u16 n <= 15, u32 n <= 30, u64 n <= 109, u128 n <= 454,
 *            big any n
 * Validated: standard, pass, 54 terms through n = 469 (2026-11-01)
 * Helpers:   gcd
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>
#include <math.h>
#include "../utils/gcd.h"

static idx_t next_shuffle(idx_t p, idx_t n)
{
    /* Convert linear index p (0‑based) to row i and column j (1‑based),
     * then compute the new index after one triangular shuffle. */
    uint64_t t = p * 8ull + 1ull;
    uint64_t root = (uint64_t)sqrt((double)t);
    idx_t i = (idx_t)((root - 1ull) / 2ull + 1ull);          /* row number (1‑based) */
    idx_t j = (idx_t)(p - (i - 1ull) * i / 2ull + 1ull);    /* column number (1‑based) */
    idx_t m = j - 1ull;                                      /* 0‑based column index */
    idx_t prev = m * (2ull * n - m + 1ull) / 2ull;           /* cards before this column */
    return prev + (n - i);                                   /* new position (0‑based) */
}

seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)1;        /* not used, but keep defined */
    idx_t T = n * (n + 1) / 2;          /* total number of cards */

    /* visited array to mark processed positions */
    bool *visited = (bool *)calloc((size_t)T, 1);
    if (!visited) return (seq_t)0;      /* allocation failure */

    seq_t order = (seq_t)1;

    for (idx_t start = 0; start < T; ++start)
    {
        if (!visited[start])
        {
            idx_t p = start;
            idx_t len = 0;
            do
            {
                visited[p] = true;
                ++len;
                p = next_shuffle(p, n);
            } while (p != start);

            seq_t l = (seq_t)len;
            seq_t g = gcd(order, l);
            order = (order / g) * l;
        }
    }

    free(visited);
    return order;
}
