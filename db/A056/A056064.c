/*
 * A056064  The Kubelsky sequence: Jack Benny's reported age, sampled
 *          annually.
 * https://oeis.org/A056064
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = min(n, 39)
 * Range:     u8 n <= 72, u16 n <= 72, u32 n <= 72, u64 n <= 72, u128 n <= 72,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n <= (idx_t)39)
        return (seq_t)n;
    else
        return (seq_t)39;
}
