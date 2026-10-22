/*
 * A007905  Conflicts during insertions into exchange trees on n nodes.
 * https://oeis.org/A007905
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = n! * Sum_{k=2..n} d(k), where d(1) = 0 and d(k) =
 *            d(floor(k/2)) + 1/(k*(k-1)) for k >= 2. Equivalently a(n) =
 *            Sum_{m=2..n} c(m,n) * n!/(m*(m-1)), where c(m,n) is the number
 *            of k in [1..n] whose binary prefix (k, k/2, k/4, ...) contains m
 * Range:     u8 n <= 5, u16 n <= 7, u32 n <= 12, u64 n <= 19, u128 n <= 20,
 *            big any n
 * Helpers:   count_desc
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include "../utils/count_desc.h"

seq_t a(idx_t n)
{
    seq_t f, s, t;
    idx_t m;
    if (n < 2) return (seq_t)0;
    f = (seq_t)1;
    for (m = 2; m <= n; m++) f *= (seq_t)m;
    s = (seq_t)0;
    for (m = 2; m <= n; m++) {
        t = f / (seq_t)m;
        t = t / (seq_t)(m - 1);
        s += t * (seq_t)count_desc(m, n);
    }
    return s;
}
