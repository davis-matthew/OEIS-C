/*
 * A189809  Number of right triangles on an (n+1) X 5 grid.
 * https://oeis.org/A189809
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 20*n^2 + 228*n - 930 for n > 15; a(1..15) are as listed
 *            in the OEIS entry
 * Range:     u8 n <= 2, u16 n <= 52, u32 n <= 200, u64 n <= 200,
 *            u128 n <= 200, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdbool.h>

static const seq_t a_vals[16] = {
    0ULL,   /* a(0) unused */
    46ULL,  158ULL,  342ULL,  596ULL,  926ULL,
    1308ULL,1754ULL, 2250ULL, 2794ULL, 3390ULL,
    4026ULL,4702ULL, 5426ULL, 6190ULL, 6994ULL
};

seq_t a(idx_t n)
{
    if (n <= 15) {
        return a_vals[(size_t)n];
    }
    seq_t nn = (seq_t)n;
    return 20 * nn * nn + 228 * nn - 930;
}
