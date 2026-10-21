/*
 * A051713  Denominator of b(n)-b(n+1), where b(n) = n/((n+1)(n+2)) =
 *          A026741/A045896.
 * https://oeis.org/A051713
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, frac, easy
 * Formula:   a(n) = ((n+1)(n+2)(n+3)) / gcd(((n+1)(n+2)(n+3)), n-1)
 * Range:     u8 n <= 4, u16 n <= 38, u32 n <= 39, u64 n <= 39, u128 n <= 39,
 *            big any n
 * Helpers:   seq_gcd
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/seq_gcd.h"

seq_t a(idx_t n)
{
    /* Compute (n+1)(n+2)(n+3) in seq_t */
    seq_t p1 = (seq_t)(n + 1);
    seq_t p2 = (seq_t)(n + 2);
    seq_t p3 = (seq_t)(n + 3);
    seq_t prod = p1 * p2 * p3;

    /* Compute gcd(prod, n-1) */
    seq_t g = seq_gcd(prod, (seq_t)(n - 1));

    /* Return denominator of the simplified fraction */
    return prod / g;
}
