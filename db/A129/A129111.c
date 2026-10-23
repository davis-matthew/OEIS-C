/*
 * A129111  Sums of three consecutive heptagonal numbers.
 * https://oeis.org/A129111
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = Hep(n)+Hep(n+1)+Hep(n+2) = (15/2)*n^2 + (21/2)*n + 8
 * Range:     u8 n <= 3, u16 n <= 91, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    seq_t nn = (seq_t)n;
    seq_t val = nn * nn * 15 + nn * 21;
    val /= 2;
    val += 8;
    return val;
}
