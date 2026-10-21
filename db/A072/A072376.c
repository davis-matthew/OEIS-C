/*
 * A072376  a(n) = a(floor(n/2)) + a(floor(n/4)) + a(floor(n/8)) + ...
 *          starting with a(0)=0 and a(1)=1.
 * https://oeis.org/A072376
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(0)=0, a(1)=1, for n>1: a(n)=msb(n)/2 = 2^⌊log₂(n)-1⌋
 * Range:     u8 n <= 511, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)0;
    if (n == 1) return (seq_t)1;
    /* compute floor(log2(n)) */
    idx_t idx = 0;
    idx_t t = n;
    while (t >>= 1) idx++;
    /* msb = 1 << idx; a(n) = msb >> 1 = 1 << (idx-1) */
    return (seq_t)1 << (idx - 1);
}
