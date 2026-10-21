/*
 * A211481  Number of ordered triples (w,x,y) with all terms in
 *          {-n,...,0,...,n} and w+2x+3y=n.
 * https://oeis.org/A211481
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = number of triples (w,x,y) with w,x,y∈[-n,n] and
 *            w+2x+3y=n. Computed by: a(n) = Σ_{y=-n}^{n} count_x(y) where
 *            count_x(y) = max(0, min(n,⌊(2n-3y)/2⌋) - max(-n,⌈(-3y)/2⌉) + 1 )
 * Range:     u8 n <= 13, u16 n <= 46, u32 n <= 46, u64 n <= 46, u128 n <= 46,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

static inline int64_t floor_div2(int64_t a)
{
    return a >= 0 ? a / 2 : -(((-a) + 1) / 2);
}

static inline int64_t ceil_div2(int64_t a)
{
    return a >= 0 ? (a + 1) / 2 : a / 2;
}

seq_t a(idx_t n)
{
    int64_t nn = (int64_t)n;
    seq_t result = 0;

    for (int64_t y = -nn; y <= nn; ++y) {
        int64_t L = -3 * y;
        int64_t R = 2 * nn - 3 * y;

        int64_t low = -nn;
        int64_t high = nn;

        int64_t ceilL = ceil_div2(L);
        int64_t floorR = floor_div2(R);

        if (ceilL > low) low = ceilL;
        if (floorR < high) high = floorR;

        int64_t count = high >= low ? high - low + 1 : 0;
        result += (seq_t)count;
    }
    return result;
}
