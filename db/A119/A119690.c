/*
 * A119690  a(n) = n! mod n*(n+1)/2.
 * https://oeis.org/A119690
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  easy, nonn, changed
 * Formula:   a(n) = n if n+1 is an odd prime, a(n) = 0 otherwise
 * Range:     u8 n <= 94, u16 n <= 94, u32 n <= 94, u64 n <= 94, u128 n <= 94,
 *            big any n
 * Helpers:   is_prime
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/is_prime.h"

seq_t a(idx_t n)
{
    if (n == 0) return 0;
    idx_t np1 = n + 1;                 /* n+1 */
    if ((np1 & 1U) == 0) return 0;    /* even --> not an odd prime */
    if (is_prime(np1))
        return (seq_t)n;
    return 0;
}
