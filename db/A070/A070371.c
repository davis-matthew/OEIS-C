/*
 * A070371  a(n) = 5^n mod 17.
 * https://oeis.org/A070371
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 5^n mod 17
 * Range:     u8 n <= 1000, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    seq_t result = 1;      /* 5^0 mod 17 = 1 */
    seq_t base   = 5;
    seq_t mod    = 17;

    while (n > 0)
    {
        if (n & 1)              /* n is odd: multiply current power */
            result = (result * base) % mod;
        base = (base * base) % mod;
        n >>= 1;                /* next bit of exponent */
    }
    return result;
}
