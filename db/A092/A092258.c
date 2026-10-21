/*
 * A092258  a(n) = (2^n-1)^(2^n).
 * https://oeis.org/A092258
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = (2^n - 1)^(2^n)
 * Range:     u8 n <= 2, u16 n <= 2, u32 n <= 3, u64 n <= 4, u128 n <= 4,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* Compute the value 2^n as a seq_t */
    seq_t two = 1;
    for (idx_t i = 0; i < n; ++i)
        two *= 2;               /* 2^n */

    /* Base of the exponentiation: 2^n - 1 */
    seq_t base = two - 1;

    /* Compute base^(2^n) by repeated squaring.
       Starting with exponent 1, each squaring doubles the exponent.
       After n squarings we have exponent 2^n. */
    seq_t result = base;
    for (idx_t i = 0; i < n; ++i)
        result *= result;

    return result;
}
