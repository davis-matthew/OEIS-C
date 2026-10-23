/*
 * A171369  Triangle read by rows, replace 2's with 3's in A169695.
 * https://oeis.org/A171369
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy, tabl
 * Formula:   a(n) = 1 if (r*(r-1)/2 + c) is a square, else a(n) = 3 where r =
 *            row number (starting at 1) and c = column within that row (also
 *            starting at 1). The sequence is read row by row. This matches
 *            OEIS A171369
 * Range:     u8 n <= 63, u16 n <= 91, u32 n <= 91, u64 n <= 91, u128 n <= 91,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>

/* Integer square root for arbitrary-precision seq_t.
   Returns floor(sqrt(x)). */
static seq_t seq_isqrt(seq_t x) {
    seq_t low = 0;
    seq_t high = x;
    while (low <= high) {
        seq_t mid = (low + high) / 2;
        seq_t sq = mid * mid;
        if (sq == x) return mid;
        if (sq < x) {
            low = mid + 1;
        } else {
            if (mid == 0) break;   /* avoid underflow when mid==0 */
            high = mid - 1;
        }
    }
    return high;
}

/* Return the n-th term of A171369 for n >= 1. */
seq_t a(idx_t n) {
    /* Find the row r such that (r-1)*r/2 < n <= r*(r+1)/2. */
    idx_t r = 0;
    idx_t sum = 0;
    for (;;) {
        ++r;
        sum += r;
        if (sum >= n) break;
    }
    /* Number of elements before this row. */
    idx_t prev = sum - r;
    /* Column index within the row (1-based). */
    idx_t k = n - prev;

    /* Compute the index value to test for square: */
    seq_t valueIndex = ((seq_t)r * (r - 1)) / 2 + (seq_t)k;

    seq_t root = seq_isqrt(valueIndex);
    if (root * root == valueIndex) {
        return (seq_t)1;
    } else {
        return (seq_t)3;
    }
}
