/*
 * A018714  Divisors of 906.
 * https://oeis.org/A018714
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, fini, full, easy
 * Formula:   a(n) is the n-th divisor of 906 in ascending order
 * Range:     u8 n <= 4, u16 n <= 8, u32 n <= 8, u64 n <= 8, u128 n <= 8,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>

/* Compute the n-th divisor of 906 in ascending order */
seq_t a(idx_t n) {
    static bool initialized = false;
    static seq_t divisors[8];

    if (!initialized) {
        idx_t idx = 0;
        /* Generate all divisors: 2^i * 3^j * 151^k for i,j,k in {0,1} */
        for (idx_t i = 0; i < 2; ++i) {
            for (idx_t j = 0; j < 2; ++j) {
                for (idx_t k = 0; k < 2; ++k) {
                    seq_t val = 1;
                    for (idx_t ii = 0; ii < i; ++ii) val *= 2;
                    for (idx_t jj = 0; jj < j; ++jj) val *= 3;
                    for (idx_t kk = 0; kk < k; ++kk) val *= 151;
                    divisors[idx++] = val;
                }
            }
        }
        /* Sort the divisors in ascending order (simple bubble sort, 8 elements) */
        idx_t total = 8;
        for (idx_t p = 0; p < total - 1; ++p) {
            for (idx_t q = p + 1; q < total; ++q) {
                if (divisors[q] < divisors[p]) {
                    seq_t tmp = divisors[p];
                    divisors[p] = divisors[q];
                    divisors[q] = tmp;
                }
            }
        }
        initialized = true;
    }
    if (n >= 1 && n <= 8) {
        return divisors[n - 1];
    }
    return 0;
}
