/*
 * A052768  a(n) = n*(n-1)*(n-2)*(n-3) for n>=5.
 * https://oeis.org/A052768
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = n*(n-1)*(n-2)*(n-3) for n >= 5; a(n) = 0 for n <= 4
 * Range:     u8 n <= 5, u16 n <= 17, u32 n <= 257, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n < 5)
        return 0;

    seq_t val = (seq_t)n;
    val = val * (seq_t)(n - 1);
    val = val * (seq_t)(n - 2);
    val = val * (seq_t)(n - 3);

    return val;
}
