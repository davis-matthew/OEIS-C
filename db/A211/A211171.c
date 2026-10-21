/*
 * A211171  Exponent of general linear group GL(n,2).
 * https://oeis.org/A211171
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 2^ceil(log_2(n)) * lcm_{i=1..n} (2^i - 1)
 * Range:     u8 n <= 3, u16 n <= 5, u32 n <= 8, u64 n <= 12, u128 n <= 18,
 *            big any n
 * Helpers:   floor_log2_uint64, gcd, pow2
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include "../utils/pow2.h"
#include "../utils/gcd.h"
#include "../utils/floor_log2_uint64.h"

seq_t a(idx_t n) {
    /* n >= 1 for the defined offset */
    if (n == 0) return 0;   /* undefined, but safe return */

    /* ceil(log2(n)) = floor_log2_uint64(n-1) + 1   for n>1, else 0 */
    idx_t ceil_log2 = (n <= 1) ? 0 : floor_log2_uint64(n - 1) + 1;

    seq_t pow_shift = pow2(ceil_log2);
    seq_t lcm_val  = 1;

    for (idx_t i = 1; i <= n; ++i) {
        seq_t term = pow2(i);
        term -= 1;                /* 2^i - 1 */

        seq_t g = gcd(lcm_val, term);
        if (g == 0) continue;     /* should not happen */

        lcm_val = (lcm_val / g) * term;
    }

    return pow_shift * lcm_val;
}
