/*
 * A104472  Triangle of degree numbers for certain polynomials.
 * https://oeis.org/A104472
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy, tabl
 * Formula:   a(n) = sum_{j=1..m} j * floor(r / j), where * r =
 *            ceil((sqrt(8*n+1)-1)/2) is the row index (number of elements in
 *            the row), * m = n - r(r-1)/2 is the column index within that
 *            row. * This implements OEIS A104472 (flattened triangle of
 *            degree numbers)
 * Range:     u8 n <= 74, u16 n <= 74, u32 n <= 74, u64 n <= 74, u128 n <= 74,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdint.h>

static idx_t find_row(idx_t k) {
    idx_t low = 0, high = 1;
    /* Double high until it covers k */
    while (high * (high + 1) / 2 < k) high <<= 1;
    /* Binary search for smallest high with high*(high+1)/2 >= k */
    while (low + 1 < high) {
        idx_t mid = (low + high) >> 1;
        if (mid * (mid + 1) / 2 >= k)
            high = mid;
        else
            low = mid;
    }
    return high;
}

seq_t a(idx_t n) {
    if (n == 0) return (seq_t)0; /* offset is 1, but safe */
    idx_t row = find_row(n);                         /* r */
    idx_t prev_tri = (row - 1) * row / 2;            /* (r-1)*r/2 */
    idx_t col = n - prev_tri;                        /* m */

    seq_t sum = (seq_t)0;
    for (idx_t j = 1; j <= col; ++j) {
        sum += (seq_t)j * (seq_t)(row / j);
    }
    return sum;
}
