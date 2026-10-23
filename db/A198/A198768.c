/*
 * A198768  a(n) = 9*5^n-1.
 * https://oeis.org/A198768
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 9*5^n-1
 * Range:     u8 n <= 2, u16 n <= 5, u32 n <= 12, u64 n <= 26, u128 n <= 53,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    seq_t result = (seq_t)1;
    seq_t base   = (seq_t)5;
    idx_t e = n;
    while (e)
    {
        if (e & 1)
            result = result * base;
        base = base * base;
        e >>= 1;
    }
    result = result * (seq_t)9;
    result = result - (seq_t)1;
    return result;
}
