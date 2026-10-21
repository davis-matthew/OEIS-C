/*
 * A070373  a(n) = 5^n mod 19.
 * https://oeis.org/A070373
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 5^n mod 19
 * Range:     u8 n <= 1000, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    seq_t result = 1;
    seq_t base   = 5 % 19;
    idx_t exp = n;
    while (exp > 0)
    {
        if (exp & 1)
            result = (result * base) % 19;
        exp >>= 1;
        if (exp)
            base = (base * base) % 19;
    }
    return result;
}
