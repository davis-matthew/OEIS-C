/*
 * A166556  Triangle read by rows, A000012 * A047999.
 * https://oeis.org/A166556
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy, tabl
 * Formula:   a(n) = T(r,k), where r is the largest integer with * r*(r+1)/2 ≤
 *            n (the row number of the flattened * coefficients matrix) and k
 *            = n - r*(r+1)/2 * T(r,k) = Σ_{j=k..r} ( binomial(j,k) mod 2 ). *
 *            In other words, a(n) is the sum of the odd binomial coefficients
 *            * in the column k of row r of Sierpins...
 * Range:     u8 n <= 5150, u16 n <= 5150, u32 n <= 5150, u64 n <= 5150,
 *            u128 n <= 5150, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include <stdint.h>

seq_t a(idx_t n) {
    /* Find the row r such that r*(r+1)/2 <= n < (r+1)*(r+2)/2.  */
    idx_t r = 0;
    /* The loop iterates O(sqrt(n)) times; acceptable for n up to 10000. */
    while (((r + 1) * (r + 2)) / 2 <= n) {
        ++r;
    }

    /* Column index within the row. */
    idx_t k = n - (r * (r + 1)) / 2;

    /* Sum the odd binomial coefficients in this column. */
    seq_t result = 0;
    for (idx_t j = k; j <= r; ++j) {
        /* binomial(j,k) is odd iff (k & (j-k)) == 0  (Lucas theorem) */
        if (((k & (j - k)) == 0U)) {
            ++result;
        }
    }

    return result;
}
