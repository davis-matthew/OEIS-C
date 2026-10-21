/*
 * A070603  a(n) = n^5 mod 19.
 * https://oeis.org/A070603
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = n^5 mod 19
 * Range:     u8 n <= 83, u16 n <= 83, u32 n <= 83, u64 n <= 83, u128 n <= 83,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    const unsigned long long M = 19ULL;
    unsigned long long r = (unsigned long long)n % M;
    unsigned long long r5 = 1;
    for (int i = 0; i < 5; ++i)
        r5 = (r5 * r) % M;
    return (seq_t)r5;
}
