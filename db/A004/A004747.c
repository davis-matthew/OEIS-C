/*
 * A004747  Triangle read by rows: the Bell transform of the triple factorial
 *          numbers A008544 without column 0.
 * https://oeis.org/A004747
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  easy, nonn, tabl
 * Formula:   T(n+1, m) = (3*n-m)*T(n, m) + T(n, m-1) for n >= m >= 1, T(n, m)
 *            = 0 for n < m, T(n, 0) = 0, T(1, 1) = 1; a(n) reads T by rows
 * Range:     u8 n <= 10, u16 n <= 21, u32 n <= 55, u64 n <= 136,
 *            u128 n <= 378, big any n
 * Helpers:   alloc_zero, release
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include "../utils/alloc_zero.h"
#include "../utils/release.h"

seq_t a(idx_t n) {
    idx_t row = 1;
    idx_t total = 1;
    while (total < n) {
        row++;
        total += row;
    }
    idx_t col = n - (total - row);

    if (row == 1) return (seq_t)1;

    seq_t *prev = alloc_zero(col + 1);
    prev[1] = 1;

    for (idx_t r = 2; r <= row; r++) {
        seq_t *curr = alloc_zero(col + 1);
        for (idx_t m = 1; m <= col; m++) {
            seq_t term1 = 0;
            if (m < r) {
                seq_t coeff = (seq_t)(3 * (r - 1) - m);
                term1 = coeff * prev[m];
            }
            seq_t term2 = 0;
            if (m > 1) term2 = prev[m - 1];
            curr[m] = term1 + term2;
        }
        release(prev, col + 1);
        prev = curr;
    }

    seq_t result = prev[col];
    release(prev, col + 1);
    return result;
}
