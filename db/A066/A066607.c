/*
 * A066607  a(n) = 3^n mod n^3.
 * https://oeis.org/A066607
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = 3^n mod n^3
 * Range:     u8 n <= 7, u16 n <= 42, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* compute 3^n modulo n^3 using binary exponentiation */
    unsigned __int128 N   = (unsigned __int128)n;
    unsigned __int128 mod = N * N * N;   /* n^3 */
    unsigned __int128 base = 3;
    unsigned __int128 result = 1;
    unsigned __int128 exp = N;           /* exponent = n */

    while (exp > 0)
    {
        if (exp & 1)
            result = (result * base) % mod;
        base = (base * base) % mod;
        exp >>= 1;
    }
    /* return the remainder cast to seq_t – truncation is allowed if the value
       does not fit in seq_t. */
    return (seq_t)result;
}
