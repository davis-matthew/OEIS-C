/*
 * A091684  a(n) = 0 if n is divisible by 3, otherwise a(n) = n.
 * https://oeis.org/A091684
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, mult, easy
 * Formula:   a(n) = 0 if n mod 3 == 0, otherwise a(n) = n
 * Range:     u8 n <= 255, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* Use idx_t for comparison (modulus is defined for unsigned types). */
    if ((n % 3) == 0)
        return 0; /* 0 in seq_t */
    else
        return (seq_t)n;
}
