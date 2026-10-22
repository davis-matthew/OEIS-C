/*
 * A001450  a(n) = binomial(5*n,2*n).
 * https://oeis.org/A001450
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = binomial(5*n, 2*n) = Product_{i=1..2n} (3n+i)/i
 * Range:     u8 n <= 2, u16 n <= 3, u32 n <= 7, u64 n <= 13, u128 n <= 27,
 *            big any n
 * Helpers:   a001450_gcd_local
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/a001450_gcd_local.h"

seq_t a(idx_t n) {
    idx_t k = 2 * n;
    idx_t base = 3 * n;
    seq_t result = 1;
    idx_t i;
    for (i = 1; i <= k; i++) {
        /* result = C(base+i-1, i-1); next = result * (base+i) / i.
           Cancel common factors first so the intermediate never exceeds
           the next value C(base+i, i), which is <= the final answer. */
        idx_t num = base + i;
        idx_t g = a001450_gcd_local(num, i);
        idx_t d = i / g;
        num /= g;
        result = (result / (seq_t)d) * (seq_t)num;
    }
    return result;
}
