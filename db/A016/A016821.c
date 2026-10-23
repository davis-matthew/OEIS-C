/*
 * A016821  a(n) = (4n+1)^9.
 * https://oeis.org/A016821
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = (4n+1)^9
 * Range:     u8 n <= 0, u16 n <= 0, u32 n <= 2, u64 n <= 17, u128 n <= 17,
 *            big any n
 * Helpers:   pow_int
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* We use the provided pow_int helper, which raises a seq_t
 * to an idx_t exponent.  For seq_t types that are
 * less than 128 bits, the result may overflow; those cases
 * are intentionally ignored in the test harness as per
 * the problem statement. */
#include <stdbool.h>
#include "../utils/pow_int.h"

seq_t a(idx_t n)
{
    /* Compute the base (4n+1) in seq_t to avoid intermediate
     * overflow on idx_t. The multiplication is performed in
     * seq_t arithmetic. */
    seq_t base = (seq_t)4 * n + 1;
    /* Raise the base to the 9th power. */
    return pow_int(base, 9);
}
