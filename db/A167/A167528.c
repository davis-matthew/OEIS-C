/*
 * A167528  a(n) = n^5 mod 50.
 * https://oeis.org/A167528
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = n^5 mod 50
 * Range:     u8 n <= 77, u16 n <= 77, u32 n <= 77, u64 n <= 77, u128 n <= 77,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    unsigned long long m = (unsigned long long)n % 50ULL;
    unsigned long long result = 1;
    unsigned long long base = m;
    unsigned int exp = 5;
    while (exp)
    {
        if (exp & 1)
            result = (result * base) % 50ULL;
        base = (base * base) % 50ULL;
        exp >>= 1;
    }
    return (seq_t)result;
}
