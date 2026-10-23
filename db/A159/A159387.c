/*
 * A159387  Number of n X n arrays of squares of integers with every
 *          (n-1)X(n-1) subblock summing to 12.
 * https://oeis.org/A159387
 *
 * Offset:    3    (a(3) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 3, u16 n <= 4, u32 n <= 6, u64 n <= 12, u128 n <= 23,
 *            big any n
 * Helpers:   is_square_small
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computation of OEIS A159387:
   a(n) = number of n×n arrays of square integers
          such that every contiguous (n‑1)×(n‑1) subblock sums to 12.
   The counting is performed by enumerating all possible
   sums of the four border groups and the interior group,
   then multiplying the numbers of assignments for each group.
   Only square values 0,1,4,9 can appear because any larger square
   would immediately violate the sum constraints. */

#include <stddef.h>
#include <stdbool.h>
#include "../utils/is_square_small.h"

/* binomial coefficient C(n,k) for 0 <= k <= n, n may be large,
   but k is always <= 12 in this problem. */
static seq_t binom(idx_t n, idx_t k)
{
    seq_t r = 1;
    for (idx_t i = 1; i <= k; ++i)
    {
        r = (r * (seq_t)(n - k + i)) / (seq_t)i;
    }
    return r;
}

/* For a group of 'cells' cells, compute the number of ways to assign
   each cell a square in {0,1,4,9} such that the sum is exactly s
   (0 <= s <= 12).  The result is stored in out[s]. */
static void count_sums(idx_t cells, seq_t out[13])
{
    for (int s = 0; s <= 12; ++s) out[s] = 0;

    for (int S = 0; S <= 12; ++S)
    {
        seq_t total = 0;
        for (int d = 0; d <= S / 9; ++d)
            for (int c = 0; c <= (S - 9 * d) / 4; ++c)
            {
                int b = S - 9 * d - 4 * c;
                int k = b + c + d;                 /* number of non‑zero cells */
                if ((idx_t)k > cells) continue;

                seq_t ways = binom(cells, k);
                ways = ways * binom(k, b);
                ways = ways * binom((idx_t)(k - b), c);
                total += ways;
            }
        out[S] = total;
    }
}

/* Main sequence function --------------------------------------------------*/
seq_t a(idx_t n)
{
    if (n < 3) return 0;       /* no defined values below offset */

    idx_t m_side = n - 2;                          /* cells in a side strip */
    idx_t m_int  = (n - 2) * (n - 2);            /* interior cells */

    seq_t side[13];           /* counts for a side strip */
    seq_t interior[13];       /* counts for the interior area */
    count_sums(m_side, side);
    count_sums(m_int,  interior);

    const int squares[4] = {0, 1, 4, 9};
    seq_t result = 0;

    /* Enumerate the only "free" corner squares w (a[1][1]) and x (a[1][n]), */
    /* the sum of the inner top row cells Smid,                      */
    /* the interior sum X,                                            */
    /* and the sum of the inner bottom row cells W.                  */
    for (int wi = 0; wi < 4; ++wi)
    {
        int w = squares[wi];
        for (int xi = 0; xi < 4; ++xi)
        {
            int x = squares[xi];

            for (int Smid = 0; Smid <= 12; ++Smid)
            {
                int A = w + Smid;          /* top row sum excluding last column */
                int B = Smid + x;          /* top row sum excluding first column */
                if (A > 12 || B > 12) continue;
                seq_t ways_top = side[Smid];
                if (!ways_top) continue;

                for (int X = 0; X <= 12; ++X)
                {
                    /* LHS sums of the side strips must be non‑negative */
                    if (X > 12 - A || X > 12 - B) continue;

                    seq_t ways_int = interior[X];
                    if (!ways_int) continue;

                    int EA = 12 - X - A;     /* left inner strip sum */
                    int FB = 12 - X - B;     /* right inner strip sum */
                    seq_t ways_left  = side[EA];
                    if (!ways_left) continue;
                    seq_t ways_right = side[FB];
                    if (!ways_right) continue;

                    for (int W = 0; W <= 12; ++W)
                    {
                        seq_t ways_bot = side[W];
                        if (!ways_bot) continue;

                        int y = A - W;   /* bottom‑left corner */
                        int z = B - W;   /* bottom‑right corner */
                        if (y < 0 || z < 0) continue;
                        if (!is_square_small(y) || !is_square_small(z)) continue;

                        seq_t prod = ways_int;
                        prod = prod * ways_top;
                        prod = prod * ways_bot;
                        prod = prod * ways_left;
                        prod = prod * ways_right;
                        result += prod;
                    }
                }
            }
        }
    }
    return result;
}
