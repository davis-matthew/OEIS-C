/*
 * A182466  a(n) = 3*a(n-1) - 2*a(n-2) with a(0)=32 and a(1)=80.
 * https://oeis.org/A182466
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 2*a(n-1)+16, a(0)=32
 * Range:     u8 n <= 2, u16 n <= 10, u32 n <= 26, u64 n <= 58, u128 n <= 122,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Equivalent closed form: a(n) = 48*2^n - 16 */

seq_t a(idx_t n)
{
    seq_t pow2 = 1;
    for (idx_t i = 0; i < n; ++i)
    {
        pow2 *= (seq_t)2;
    }
    return (seq_t)48 * pow2 - (seq_t)16;
}
