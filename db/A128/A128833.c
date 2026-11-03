/*
 * A128833  Number of n-tuples where each entry is chosen from the subsets of
 *          {1,2,3,4,5} such that the intersection of all n entries is empty.
 * https://oeis.org/A128833
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = (2^n-1)^5
 * Range:     u8 n <= 2, u16 n <= 3, u32 n <= 6, u64 n <= 12, u128 n <= 25,
 *            big any n
 * Validated: standard, pass, 31 terms through n = 638 (2026-11-01)
 * Helpers:   ipow_i32, pow2
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include <stdint.h>
#include "../utils/ipow_i32.h"
#include "../utils/pow2.h"

seq_t a(idx_t n) {
    /* compute 2^n */
    seq_t t = pow2(n);
    /* subtract 1 to get 2^n - 1 */
    t -= 1;
    /* raise to the 5th power */
    return ipow_i32(t, 5);
}
