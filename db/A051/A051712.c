/*
 * A051712  Numerator of b(n)-b(n+1), where b(n) = n/((n+1)(n+2)) =
 *          A026741/A045896.
 * https://oeis.org/A051712
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, frac, easy, mult
 * Formula:   not stated
 * Range:     u8 n <= 256, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   gcd_seq
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A051712: numerator of b(n)-b(n+1) where b(n)=n/((n+1)(n+2))
 * Formula: a(n) = (n-1) / gcd( n-1, (n+1)(n+2)(n+3) )
 * This implementation reduces the fraction by
 * sequentially dividing (n-1) by gcd with each of n+1, n+2 and n+3.
 */

#include <stdint.h>
#include "../utils/gcd_seq.h"

seq_t a(idx_t n)
{
    seq_t a_val = (seq_t)n - 1;
    seq_t g;

    g = gcd_seq(a_val, (seq_t)n + 1);
    if (g != 0)
        a_val /= g;

    g = gcd_seq(a_val, (seq_t)n + 2);
    if (g != 0)
        a_val /= g;

    g = gcd_seq(a_val, (seq_t)n + 3);
    if (g != 0)
        a_val /= g;

    return a_val;
}
