/*
 * A074800  a(n) = denominator( (4*n+1)*(Product_{i=1..n}
 *          (2*i-1)/Product_{i=1..n} (2*i))^5 ).
 * https://oeis.org/A074800
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, frac, nonn
 * Formula:   a(n) = 2^(10*n - 5*popcount(n)) , where popcount is the number
 *            of 1‑bits of n
 * Range:     u8 n <= 1, u16 n <= 2, u32 n <= 3, u64 n <= 7, u128 n <= 14,
 *            big any n
 * Helpers:   pow2
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include "../utils/pow2.h"

seq_t a(idx_t n) {
    /* Compute the popcount of n (number of bits set to 1). */
    idx_t pc = 0;
    idx_t m = n;  /* copy to keep original value for 10*n */
    while (m) {
        pc += m & 1ULL;
        m >>= 1U;
    }
    /* Compute the exponent: 10*n - 5*popcount(n). */
    idx_t exp = 10ULL * n - 5ULL * pc;
    /* Return 2^exp.  pow2 is defined in utils/pow2.h. */
    return pow2(exp);
}
