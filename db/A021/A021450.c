/*
 * A021450  Decimal expansion of 1/446.
 * https://oeis.org/A021450
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, cons, easy
 * Formula:   a(n) = ⌊10^(n+1) / 446⌋ mod 10
 * Range:     u8 n <= 98, u16 n <= 98, u32 n <= 98, u64 n <= 98, u128 n <= 98,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* a(n) is the (n‑th) digit after the decimal point in the expansion of 1/446. */

/* The following implementation performs a single pass of the usual
 * long division algorithm.  It always works in O(n) time and uses only
 * integer arithmetic with intermediate values bounded by 4460,
 * which fits in standard integer types.  It does not rely on any
 * external library or memoisation and therefore is safe for arbitrary
 * calling order. */
#include <stdint.h>

seq_t a(idx_t n)
{
    /* r keeps the current remainder (always < 446).  Use a wide type
     * for safety; idx_t is at least uint64_t. */
    idx_t r = 1;
    seq_t digit = 0;

    for (idx_t i = 0; i <= n; ++i) {
        /* Multiply remainder by 10, obtain the next digit, and reduce the remainder. */
        r *= 10;
        digit = (seq_t)(r / 446);
        r    = r % 446;
    }
    return digit;
}
