/*
 * A175454  a(n) = number of divisors of n(n+1) that divide neither n nor n+1.
 * https://oeis.org/A175454
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = (d(n)-1)*(d(n+1)-1)
 * Range:     u8 n <= 2078, u16 n <= 9174, u32 n <= 9174, u64 n <= 9174,
 *            u128 n <= 9174, big any n
 * Helpers:   divisor_count_u64
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* where d(k) is the number of divisors of k */

#include <stdint.h>
#include "../utils/divisor_count_u64.h"

seq_t a(idx_t n) {
    if (n < 1) return 0;
    /* divisor_count_u64 returns a seq_t */
    seq_t dn  = divisor_count_u64((uint64_t)n);
    seq_t dn1 = divisor_count_u64((uint64_t)(n + 1));
    seq_t part1 = dn - (seq_t)1;
    seq_t part2 = dn1 - (seq_t)1;
    return part1 * part2;
}
