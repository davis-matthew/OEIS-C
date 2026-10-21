/*
 * A232245  Sum of the number of ones in binary representation of n and n^2.
 * https://oeis.org/A232245
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, base
 * Formula:   a(n) = popcount(n) + popcount(n^2)
 * Range:     u8 n <= 15, u16 n <= 81, u32 n <= 81, u64 n <= 81, u128 n <= 81,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

static inline seq_t popcount_seq_t(seq_t x)
{
    seq_t v = x;
    seq_t c = 0;
    const seq_t one = (seq_t)1;
    while (v != (seq_t)0)
    {
        c += v & one;
        v >>= 1;
    }
    return c;
}

seq_t a(idx_t n)
{
    seq_t tn  = (seq_t)n;
    seq_t tn2 = tn * tn;
    return popcount_seq_t(tn) + popcount_seq_t(tn2);
}
