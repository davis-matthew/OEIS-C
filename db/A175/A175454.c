/*
 * A175454  a(n) = number of divisors of n(n+1) that divide neither n nor n+1.
 * https://oeis.org/A175454
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = (d(n)-1)*(d(n+1)-1)
 * Range:     u8 n <= 2078, u16 n <= 16385, u32 n <= 16385, u64 n <= 16385,
 *            u128 n <= 16385, big any n
 * Validated: standard, pass, 104 terms through n = 9174 (2026-11-01)
 * Helpers:   num_divisors
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* where d(k) is the number of divisors of k */

#include <stdint.h>
#include "../utils/num_divisors.h"

seq_t a(idx_t n) {
    if (n < 1) return 0;
    /* divisor_count_u64 returns a seq_t */
    seq_t dn  = num_divisors((uint64_t)n);
    seq_t dn1 = num_divisors((uint64_t)(n + 1));
    seq_t part1 = dn - (seq_t)1;
    seq_t part2 = dn1 - (seq_t)1;
    return part1 * part2;
}
