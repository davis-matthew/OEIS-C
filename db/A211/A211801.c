/*
 * A211801  Number of ordered triples (w,x,y) with all terms in {1,...,n} and
 *          2w^3<x^3+y^3.
 * https://oeis.org/A211801
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = # of ordered triples (w,x,y) with all terms in {1,…,n}
 *            that satisfy 2·w³ < x³ + y³. The implementation counts the
 *            complement for each fixed w by a two‑pointer scan over x and y
 * Range:     u8 n <= 5, u16 n <= 31, u32 n <= 40, u64 n <= 40, u128 n <= 40,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n)
{
    /* Reuse a pre‑allocated cube table across calls. */
    static seq_t *cubes = NULL;
    static idx_t cubes_cap = 0;

    if (n > cubes_cap) {
        free(cubes);
        cubes = (seq_t *)malloc((n + 1) * sizeof(seq_t));
        cubes_cap = n;
    }

    /* Precompute i³ for i=1…n. */
    for (idx_t i = 1; i <= n; ++i)
        cubes[i] = (seq_t)i * i * i;

    const seq_t total_pairs = (seq_t)n * n;
    seq_t total = 0;

    for (idx_t w = 1; w <= n; ++w) {
        const seq_t threshold = (seq_t)2 * cubes[w];   /* 2w³ */
        idx_t y = n;
        seq_t count_le = 0;  /* pairs with x³ + y³ ≤ threshold */

        for (idx_t x = 1; x <= n; ++x) {
            const seq_t cx = cubes[x];
            /* Decrease y while cx + cubes[y] exceeds threshold. */
            while (y > 0 && cx + cubes[y] > threshold)
                --y;
            count_le += y;    /* all y=1…y satisfy cx + cubes[y] ≤ threshold */
            if (y == 0)       /* no larger y can satisfy any later x */
                break;
        }

        const seq_t pairs_gt = total_pairs - count_le;
        total += pairs_gt;
    }

    return total;
}
