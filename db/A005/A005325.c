/*
 * A005325  Column of Motzkin triangle.
 * https://oeis.org/A005325
 *
 * Offset:    5    (a(5) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 6*sum(j=ceiling((n-5)/2)..(n+1),
 *            C(j,2*j-n+5)*C(n+1,j))/(n+1), evaluated as a(n) = 6/(n+1) *
 *            sum_{i=0..floor(m/2)} t(i), m = n-5, t(0) = C(n+1,6), t(i+1) =
 *            t(i)*(m-2i)*(m-2i-1)/((i+1)*(i+7)). For fixed-width seq_t (no
 *            exact division possible after wraparound) it instead counts
 *            Motzkin path...
 * Range:     u8 n <= 8, u16 n <= 13, u32 n <= 23, u64 n <= 44, u128 n <= 85,
 *            big any n
 * Helpers:   a_paths, a_sum, seq_is_big
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include "../utils/seq_is_big.h"
#include "../utils/a_paths.h"
#include "../utils/a_sum.h"

seq_t a(idx_t n) {
    if (n < 5) return 0;
    if (seq_is_big()) return a_sum(n);
    return a_paths(n);
}
