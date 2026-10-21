/*
 * A065413  Number of positive solutions to "numbers that are n times their
 *          number of binary 1's".
 * https://oeis.org/A065413
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  base, nonn
 * Formula:   a(n) = |{ t ≥ 1 : popcount(n * t) = t }|
 * Range:     u8 n <= 52, u16 n <= 7721, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    seq_t result = 0;
    seq_t ns = (seq_t)n;
    for (idx_t t = 1; ; ++t)
    {
        seq_t m = ns * (seq_t)t;
        idx_t pop  = 0;
        idx_t bits = 0;
        seq_t tmp = m;
        while (tmp != (seq_t)0)
        {
            if ((tmp & (seq_t)1) != (seq_t)0) ++pop;
            ++bits;
            tmp >>= 1;
        }
        if (pop == t) ++result;
        if (t > bits) break;
    }
    return result;
}
