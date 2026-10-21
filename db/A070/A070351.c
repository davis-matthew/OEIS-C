/*
 * A070351  a(n) = 2^n mod 47.
 * https://oeis.org/A070351
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 2^n mod 47
 * Range:     u8 n <= 1000, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    seq_t res = 1;
    seq_t base = 2 % 47;
    idx_t e = n;
    while (e)
    {
        if (e & 1)
            res = (res * base) % 47;
        base = (base * base) % 47;
        e >>= 1;
    }
    return res;
}
