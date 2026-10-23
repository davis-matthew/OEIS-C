/*
 * A115001  a(n) = Sum_{k=1..n} J_2(k)/24.
 * https://oeis.org/A115001
 *
 * Offset:    4    (a(4) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Sum_{k=1..n} J_2(k) / 24 * where J_2(k) is the Jordan
 *            totient function of order 2: * J_2(k) = k^2 * ∏_{p|k} (1 -
 *            1/p^2) (product over distinct primes p dividing k)
 * Range:     u8 n <= 9, u16 n <= 161, u32 n <= 816, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   jordan2
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include "../utils/jordan2.h"

seq_t a(idx_t n)
{
    /* Sum J_2(k) for k = 1..n and divide by 24 */
    seq_t sum = 0;
    for (idx_t k = 1; k <= n; ++k)
        sum += jordan2(k);
    return sum / 24;
}
