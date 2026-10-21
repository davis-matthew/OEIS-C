/*
 * A099197  a(n) = n^2*(2*n^8 + 120*n^6 + 1806*n^4 + 7180*n^2 + 5067)/14175.
 * https://oeis.org/A099197
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = n^2*(2*n^8 + 120*n^6 + 1806*n^4 + 7180*n^2 + 5067)/14175
 * Range:     u8 n <= 3, u16 n <= 6, u32 n <= 22, u64 n <= 204,
 *            u128 n <= 10000, big any n
 * Helpers:   binomial
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/binomial.h"

seq_t a(idx_t n) {
    if (n == 0) return 0;
    seq_t sum = 0;
    for (idx_t j = 0; j < 10 && j < n; ++j) {
        seq_t coeff = binomial(9, j);
        seq_t term  = binomial(n - j + 9, 10);
        sum += coeff * term;
    }
    return sum;
}
