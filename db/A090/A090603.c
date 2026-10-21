/*
 * A090603  Number of n-element labeled groupoids with a fixed identity.
 * https://oeis.org/A090603
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, changed
 * Formula:   a(n) = n^((n-1)^2)
 * Range:     u8 n <= 3, u16 n <= 3, u32 n <= 4, u64 n <= 5, u128 n <= 7,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>   /* for idx_t and seq_t types (supplied by the build) */

/* Compute base^exp using exponentiation by squaring.
   Works for any seq_t type that supports +, *, and assignment. */
static seq_t pow_seq_t(seq_t base, idx_t exp)
{
    seq_t result = (seq_t)1;
    while (exp) {
        if (exp & 1)
            result *= base;
        base *= base;
        exp >>= 1;
    }
    return result;
}

seq_t a(idx_t n)
{
    /* The sequence is defined for n ≥ 1; values for n = 0 are undefined. */
    if (n == 0)
        return 0;

    /* exponent = (n-1)^2 */
    idx_t tmp = n - 1;
    idx_t exp = tmp * tmp;

    return pow_seq_t((seq_t)n, exp);
}
