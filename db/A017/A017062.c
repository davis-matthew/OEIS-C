/*
 * A017062  a(n) = (7*n + 6)^10.
 * https://oeis.org/A017062
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = (7*n + 6)^10
 * Range:     u8 none, u16 none, u32 n <= 0, u64 n <= 11, u128 n <= 1017,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>  /* allowed but not strictly necessary */

seq_t a(idx_t n)
{
    /* Compute the base as 7*n + 6, using seq_t arithmetic */
    seq_t base = ((seq_t)n) * (seq_t)7 + (seq_t)6;

    /* Fast exponentiation: raise base to the 10th power */
    seq_t result = (seq_t)1;
    idx_t exp = (idx_t)10;
    seq_t power = base;

    while (exp > (idx_t)0) {
        if ((exp & (idx_t)1) != (idx_t)0) {
            result = result * power;
        }
        power = power * power;
        exp >>= (idx_t)1;
    }

    return result;
}
