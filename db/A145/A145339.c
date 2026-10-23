/*
 * A145339  a(n) = the minimum of d(p(n)-1) and d(p(n)+1), where d(m) is the
 *          number of divisors of m and p(n) is the n-th prime.
 * https://oeis.org/A145339
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = min(d(p(n)-1), d(p(n)+1)) where d(m) is the number of
 *            divisors of m and p(n) is the n-th prime
 * Range:     u8 n <= 101, u16 n <= 101, u32 n <= 101, u64 n <= 101,
 *            u128 n <= 101, big any n
 * Helpers:   divisor_count_u64, get_prime
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include "../utils/divisor_count_u64.h"
#include "../utils/get_prime.h"

seq_t a(idx_t n)
{
    uint64_t p     = get_prime(n);            /* n-th prime (1‑indexed) */
    seq_t   d1     = divisor_count_u64(p - 1);/* divisor count of p-1   */
    seq_t   d2     = divisor_count_u64(p + 1);/* divisor count of p+1   */
    return (d1 < d2) ? d1 : d2;
}
