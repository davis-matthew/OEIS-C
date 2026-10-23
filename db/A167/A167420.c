/*
 * A167420  a(n) = 2^n mod 14.
 * https://oeis.org/A167420
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 2^n mod 14
 * Range:     u8 n <= 99, u16 n <= 99, u32 n <= 99, u64 n <= 99, u128 n <= 99,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* Working modulo 14.  The intermediate product never exceeds 14*14 = 196,
       which fits in any unsigned integer type used for seq_t. */
    seq_t result = 1 % 14;
    seq_t base   = 2 % 14;
    while (n > 0)
    {
        if (n & 1ULL)
            result = (result * base) % 14;
        base = (base * base) % 14;
        n >>= 1;
    }
    return result;
}
