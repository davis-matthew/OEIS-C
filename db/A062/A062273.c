/*
 * A062273  a(n) is an n-digit number with digits in increasing order with 0
 *          following 9 and this is maintained in the concatenation of any
 *          number of consecutive terms.
 * https://oeis.org/A062273
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base, easy
 * Formula:   a(n) = Σ_{i=1..n} ((n*(n-1)/2 + i) mod 10) * 10^(n-i)
 * Range:     u8 n <= 2, u16 n <= 5, u32 n <= 9, u64 n <= 20, u128 n <= 39,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

seq_t a(idx_t n)
{
    if (n == 0)
        return (seq_t)0; /* undefined for n < 1, return 0 */

    /* Compute the base offset modulo 10 */
    seq_t base = ((seq_t)n * ((seq_t)n - 1) / 2) % 10U;

    seq_t result = 0;
    for (seq_t i = 1; i <= (seq_t)n; ++i) {
        seq_t digit = (base + i) % 10U;
        result = result * 10U + digit;
    }
    return result;
}
