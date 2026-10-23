/*
 * A168400  a(n) = 3^n mod 15.
 * https://oeis.org/A168400
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 3^n mod 15
 * Range:     u8 n <= 1000, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    seq_t base = (seq_t)3 % (seq_t)15;
    seq_t result = 1;
    idx_t k = n;
    seq_t mod = (seq_t)15;
    while (k)
    {
        if (k & 1)
            result = (result * base) % mod;
        base = (base * base) % mod;
        k >>= 1;
    }
    return result;
}
