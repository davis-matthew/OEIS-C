/*
 * A119980  Order of the following permutation on 3n+1 symbols. Write the 3n+1
 *          symbols horizontally into a 3-column grid and read them off
 *          vertically, i.e., column after column.
 * https://oeis.org/A119980
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = order of the permutation obtained by writing numbers
 *            0..3n in a 3‑column grid row­wise and reading them column­wise
 * Range:     u8 n <= 9, u16 n <= 33, u32 n <= 117, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   _gcd_seq
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdbool.h>
#include <stdint.h>
#include "../utils/_gcd_seq.h"

/* Euclidean algorithm for the (unsigned) type seq_t.  Works for
   built‑in unsigned types, __int128, and for the arbitrary‑precision
   integer class used by the build. */

seq_t a(idx_t n)
{
    /* number of symbols = 3n + 1 */
    idx_t N = 3 * n + (idx_t)1;

    /* visited array for cycle detection */
    unsigned char *vis = (unsigned char *)malloc((size_t)N);
    if (!vis)
        return (seq_t)0;              /* allocation failure unlikely */

    for (idx_t i = 0; i < N; ++i)
        vis[i] = 0;

    seq_t order = (seq_t)1;

    for (idx_t start = 0; start < N; ++start)
    {
        if (vis[start])
            continue;

        /* Follow the cycle starting at 'start' */
        idx_t len = 0;
        idx_t cur = start;

        do
        {
            vis[cur] = 1;
            idx_t r = cur / 3;
            idx_t c = cur % 3;

            switch (c)
            {
            case 0:
                cur = r;                  /* column 0 */
                break;
            case 1:
                cur = (n + (idx_t)1) + r; /* column 1 */
                break;
            case 2:
                cur = (2 * n + (idx_t)1) + r; /* column 2 */
                break;
            }
            ++len;
        } while (cur != start);

        /* Update lcm(order, len) */
        seq_t len_seq = (seq_t)len;
        seq_t g = _gcd_seq(order, len_seq);
        order = (order / g) * len_seq;
    }

    free(vis);
    return order;
}
