/*
 * A063088  a(n) = Sum_{k=1..n} phi(k) - Sum{k=1..n} d(k), where d() is the
 *          number of divisors function.
 * https://oeis.org/A063088
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  easy, sign
 * Formula:   a(n) = Sum_{k=1..n} phi(k) - Sum_{k=1..n} d(k)
 * Range:     i8 n <= 26, i16 n <= 337, i32 n <= 1000, i64 n <= 1000,
 *            i128 n <= 1000, big any n
 * Helpers:   divisor_count_u64, phi
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include "../utils/phi.h"
#include "../utils/divisor_count_u64.h"

seq_t a(idx_t n)
{
    seq_t sum_phi = 0;
    seq_t sum_d   = 0;

    for (idx_t k = 1; k <= n; ++k) {
        sum_phi += phi(k);
        sum_d += (seq_t)divisor_count_u64(k);
    }

    return sum_phi - sum_d;
}
