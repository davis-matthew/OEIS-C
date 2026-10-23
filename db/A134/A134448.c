/*
 * A134448  a(n) = discriminant of Brioschi quintic polynomial x^5 - 10*n*x^3
 *          + 45*n^2*x - n^2.
 * https://oeis.org/A134448
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 3125 * n^8 * (1728*n - 1)^2
 * Range:     u8 none, u16 none, u32 none, u64 n <= 8, u128 n <= 13, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n == 0) return 0;
    seq_t sn = (seq_t)n;
    seq_t n2 = sn * sn;
    seq_t n4 = n2 * n2;
    seq_t n8 = n4 * n4;

    seq_t t = (seq_t)1728;
    seq_t tn = t * sn;
    seq_t diff = tn - (seq_t)1;
    seq_t diff2 = diff * diff;

    seq_t result = (seq_t)3125 * n8 * diff2;
    return result;
}
