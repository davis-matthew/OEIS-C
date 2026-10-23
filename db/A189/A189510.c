/*
 * A189510  Digital root of n^n.
 * https://oeis.org/A189510
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, base, easy
 * Formula:   a(n) = digital_root(n^n)
 * Range:     u8 n <= 90, u16 n <= 90, u32 n <= 90, u64 n <= 90, u128 n <= 90,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n == 0)
        return 1; /* 0^0 is defined as 1 */

    const int mod = 9;
    int base  = (int)(n % mod);
    idx_t exp = n;
    int  result = 1;

    while (exp > 0)
    {
        if (exp & 1)
            result = (result * base) % mod;
        base = (base * base) % mod;
        exp >>= 1;
    }

    int dr = (result == 0) ? 9 : result;
    return (seq_t)dr;
}
