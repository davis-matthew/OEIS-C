/*
 * A027868  Number of trailing zeros in n!; highest power of 5 dividing n!.
 * https://oeis.org/A027868
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, base, nice, easy
 * Formula:   a(n) = Sum_{i>=1} floor(n/5^i) (OEIS A027868)
 * Range:     u8 n <= 1029, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    seq_t sum = 0;
    idx_t t = n;
    while (t >= 5)
    {
        t /= 5;
        sum += (seq_t)t;
    }
    return sum;
}
