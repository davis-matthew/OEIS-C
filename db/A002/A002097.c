/*
 * A002097  Numbers that are not the sum of 3 nonzero triangular numbers.
 * https://oeis.org/A002097
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  fini, full, nonn
 * Formula:   count numbers not representable as sum of 3 nonzero triangular
 *            numbers
 * Range:     u8 n <= 7, u16 n <= 7, u32 n <= 7, u64 n <= 7, u128 n <= 7,
 *            big any n
 * Helpers:   is_sum_of_three_triangular, is_triangular, isqrt_newton
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include "../utils/is_sum_of_three_triangular.h"
#include "../utils/is_triangular.h"
#include "../utils/isqrt_newton.h"

seq_t a(idx_t n) {
    idx_t count = 0;
    seq_t cand = 1;
    
    while (cand <= (seq_t)100000) {
        if (!is_sum_of_three_triangular(cand)) {
            count++;
            if (count == n) return cand;
        }
        cand++;
    }
    
    return 0;
}
