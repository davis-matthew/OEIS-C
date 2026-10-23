/*
 * A025766  Expansion of 1/((1-x)*(1-x^2)*(1-x^11)).
 * https://oeis.org/A025766
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = floor((n^2 + 14*n + 56)/44) + [(n mod 22)=4]
 * Range:     u8 n <= 8, u16 n <= 58, u32 n <= 58, u64 n <= 58, u128 n <= 58,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    seq_t nn = (seq_t)n;
    seq_t num = nn * (nn + (seq_t)14) + (seq_t)56;
    seq_t val = num / (seq_t)44;
    if ((n % 22) == 4u)
        val += (seq_t)1;
    return val;
}
