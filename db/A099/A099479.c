/*
 * A099479  Count, repeating 4n three times for n > 0.
 * https://oeis.org/A099479
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = floor((n+2)/3) + floor((n+3)/6) + floor((n+4)/6)
 * Range:     u8 n <= 380, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    seq_t ns = (seq_t)n;
    seq_t t0 = (ns + (seq_t)2) / 3;
    seq_t t1 = (ns + (seq_t)3) / 6;
    seq_t t2 = (ns + (seq_t)4) / 6;
    return t0 + t1 + t2;
}
