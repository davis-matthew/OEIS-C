/*
 * A175363  Partial sums of A175362.
 * https://oeis.org/A175363
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = #{(x,y)∈ℤ² : |x|³+|y|³ ≤ n}
 * Range:     u8 n <= 71, u16 n <= 71, u32 n <= 71, u64 n <= 71, u128 n <= 71,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

/* integer floor cube root; works for idx_t up to 2⁶⁴‑1 */
static idx_t
floor_cuberoot(idx_t n)
{
    idx_t r = 0;
    while ((r + 1) * (r + 1) * (r + 1) <= n)
        ++r;
    return r;
}

/* number of pairs (x,y) with |x|³+|y|³ ≤ n */
static seq_t
count_pairs(idx_t n)
{
    seq_t total = 0;
    idx_t xmax = floor_cuberoot(n);
    for (idx_t x = 0; x <= xmax; ++x)
    {
        idx_t rem = n - (x * x * x);
        idx_t ymax = floor_cuberoot(rem);
        if (x == 0)
            total += (seq_t)2 * ymax + 1;      /* (0,0) and (0,±y) */
        else
            total += (seq_t)4 * ymax + 2;      /* (±x,±y) for y>0 and (±x,0) */
    }
    return total;
}

/* Sequence a(n) – the required OEIS value */
seq_t a(idx_t n)
{
    return count_pairs(n);
}
