/*
 * A217984  Number of divisors of n^3 + 1 of the form k^3 + 1.
 * https://oeis.org/A217984
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = |{k ≥ 0 : k^3+1 divides n^3+1}|
 * Range:     u8 n <= 10000, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    __int128 nn = n;
    __int128 m128 = nn*nn*nn + 1;
    seq_t count = 0;
    for (idx_t k = 0; ; ++k)
    {
        __int128 kd = k;
        __int128 d128 = kd*kd*kd + 1;
        if (d128 > m128) break;
        if (m128 % d128 == 0) ++count;
    }
    return count;
}
