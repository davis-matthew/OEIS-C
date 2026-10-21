/*
 * A070380  a(n) = 5^n mod 32.
 * https://oeis.org/A070380
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 5^n mod 32
 * Range:     u8 n <= 1000, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    seq_t result = 1;
    seq_t base   = 5;
    idx_t  exp   = n;

    while (exp > 0)
    {
        if (exp & 1)
            result = (result * base) % (seq_t)32;
        base = (base * base) % (seq_t)32;
        exp >>= 1;
    }
    return result;
}
