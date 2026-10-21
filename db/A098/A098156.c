/*
 * A098156  Interleave n+1 and 2n+1 and take binomial transform.
 * https://oeis.org/A098156
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(0)=1, a(1)=2, a(2)=5; for n≥3 a(n)= (3*n+4) << (n-3)
 * Range:     u8 n <= 6, u16 n <= 13, u32 n <= 28, u64 n <= 59, u128 n <= 122,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)1;
    if (n == 1) return (seq_t)2;
    if (n == 2) return (seq_t)5;
    seq_t base = (seq_t)(3 * n + 4);
    return base << (unsigned)(n - 3);
}
